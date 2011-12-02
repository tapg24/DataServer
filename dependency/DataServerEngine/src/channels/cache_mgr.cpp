#include "channels/cache_mgr.h"
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/log/trivial.hpp>

namespace Channels
{
	CacheMgr::CacheMgr()
	{
		notifyThread = boost::thread(boost::bind( &CacheMgr::NotifyCycleThread, this ) );
	}

	CacheMgr::~CacheMgr()
	{
		Clear();
	}

	boost::signals2::connection CacheMgr::Bind(const Slot& slot)
	{
		{
			boost::mutex::scoped_lock lock(cacheGuard_);

			std::list<std::string> changedList;
			CacheContainer::const_iterator iter = cache_.begin();
			for(; iter != cache_.end(); ++iter)
			{
				changedList.push_back( iter->first );
			}
			slot(changedList);
		}

		return onNotify.connect(slot);
	}

	void CacheMgr::Bind( const boost::signals2::connection& connection )
	{
		onNotify.disconnect(connection);
	}

	void CacheMgr::NotifyCycleThread()
	{
		while( !stopCycle.timedWait( 10 ) )
		{
			std::list<std::string> changedList;

			CacheContainer::const_iterator iter = cache_.begin();
			for(; iter != cache_.end(); ++iter)
			{
				if( iter->second->isChanged() )
				{
					changedList.push_back( iter->first );
				}
			}

			if ( !changedList.empty() )
			{
				onNotify(changedList);
			}
		}
	}

	void CacheMgr::OnNotifyTimer()
	{
	}

	bool CacheMgr::ItemExist( const std::string& name ) const
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		CacheContainer::const_iterator where = cache_.find(name);
		if( where != cache_.end() )
		{
			return true;
		}
		
		return false;
	}

	void CacheMgr::UpdateItem( const CacheItemPtr& itemPtr )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		CacheContainer::const_iterator where = cache_.find(itemPtr->GetName());
		BOOST_ASSERT( where != cache_.end() );
		const CacheItem& item = boost::cref(*itemPtr.get());
		where->second->CopyFrom(item);
	}

	CacheItemPtr CacheMgr::GetItem(const std::string& itemName)
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		CacheContainer::iterator where = cache_.find(itemName);
		BOOST_ASSERT( where != cache_.end() );

		return where->second;
	}

	void CacheMgr::InsertItem( const std::string& name, VARTYPE type )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		ComVariant newVal;
		newVal.setType(type);
		CacheItemPtr newItem(new CacheItem(name, newVal));
		cache_.insert(std::make_pair(name, newItem));
	}

	void CacheMgr::InsertItem( const std::string& name, const VARIANT& variant, const WORD quality )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		CacheItemPtr newItem(new CacheItem(name, variant, quality));
		cache_.insert(std::make_pair(name, newItem));
	}

	void CacheMgr::InsertItem( const TagInfo& tag )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		ComVariant newVal;
		newVal.setType(tag.type_);
		CacheItemPtr newItem(new CacheItem(tag.name_, newVal));
		cache_.insert(std::make_pair(tag.name_, newItem));
	}

	void CacheMgr::NotifyAll() const
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		std::list<std::string> changedList;
		CacheContainer::const_iterator iter = cache_.begin();
		for(; iter != cache_.end(); ++iter)
		{
			changedList.push_back( iter->first );
		}

		if ( !changedList.empty() )
		{
			onNotify(changedList);
		}
	}

	void CacheMgr::NotifyThis(const std::list<std::string>& changedList) const
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		if ( !changedList.empty() )
		{
			onNotify(changedList);
		}
	}

	void CacheMgr::ResetAll( )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		BOOST_FOREACH(CacheItemPtr& itemPtr, cache_ | boost::adaptors::map_values)
		{
			itemPtr->Reset();
		}
	}

	void CacheMgr::ResetThis( const std::list<std::string>& resetList )
	{
		boost::mutex::scoped_lock lock(cacheGuard_);

		BOOST_FOREACH(const std::string& name, resetList)
		{
			CacheContainer::iterator iter = cache_.find(name);
			if ( iter != cache_.end() )
			{
				iter->second->Reset();
			}
		}
	}

	void CacheMgr::Clear()
	{
		onNotify.disconnect_all_slots();
		stopCycle.signal();
		notifyThread.join();
	}

}