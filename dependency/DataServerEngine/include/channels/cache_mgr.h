#ifndef CACHE_MGR_H
#define CACHE_MGR_H

#include "channels/tag_info.h"
#include "cache_item.h"

#include "utils/singleton.h"
#include "utils/event.h"
#include <string>
#include <map>
#include <list>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

class CacheItem;

namespace Channels
{
	class CacheMgr : boost::noncopyable
	{
		//typedef boost::function< void( const CacheContainer& ) > DataCallbackType;
		typedef boost::signals2::signal< void ( const std::list<std::string>& ) > Signal;
		typedef Signal::slot_type Slot;

		Signal onNotify;

		// members
		CacheContainer cache_;
		Event stopCycle;
		boost::thread notifyThread;
		mutable boost::mutex notifyMutex;
		boost::mutex getMutex;
		boost::mutex updateMutex;
		boost::mutex insertMutex;
		boost::mutex subscribeMutex;

	private:
		void NotifyCycleThread();
		void OnNotifyTimer();

	public:
		CacheMgr();
		~CacheMgr();

		boost::signals2::connection Bind(const Slot& slot);
		void Bind(const boost::signals2::connection& connection);
		void NotifyAll() const;
		void NotifyThis(const std::list<std::string>& changeList) const;
		void ResetAll();
		void ResetThis(const std::list<std::string>& resetList);
		bool ItemExist(const std::string& name) const;
		void UpdateItem(const CacheItemPtr& itemPtr);

		void InsertItem(const std::string& name, VARTYPE type);
		void InsertItem(const std::string& name, const VARIANT& variant, const WORD quality);
		void InsertItem(const TagInfo& tag);

		//template <typename Type>
		//void InsertItem(const std::string& itemName)
		//{
		//	Type newVal = 0;
		//	CacheItemPtr newItem(new CacheItem(itemName, Variant(newVal)));
		//	cacheContainer.insert(std::make_pair(itemName, newItem));
		//}

		CacheItemPtr GetItem(const std::string& itemName);
	};
}

//typedef Singleton< Channels::CacheMgr > cacheMgr;

#endif // CACHE_MGR_H