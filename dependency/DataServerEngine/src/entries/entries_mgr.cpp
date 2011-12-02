#include "entries/entries_mgr.h"
#include "entries/entry.h"
#include <memory>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

namespace Channels
{
	EntriesMgr::EntriesMgr()
	{
	}

	EntriesMgr::~EntriesMgr()
	{
	}

	void EntriesMgr::AddEntry( EntryPtr entry )
	{
		if ( entry->GetType() == NullEntry_Type )
		{
			return;
		}

		entries_.insert( std::make_pair(entry->GetName(), entry) );
		BOOST_FOREACH(const string_t& src, entry->GetSrcList())
		{
			src_assoc_.insert( std::make_pair(src, entry) );
		}
	}

	EntryPtr EntriesMgr::GetEntryByDst( const string_t& dst_name )
	{
		string_t dst = dst_name;
		if ( boost::contains( dst_name, "_CMD" ) )
		{
			dst = boost::replace_last_copy(dst_name, "_CMD", "_TS");
		}

		BOOST_AUTO(entry_iter, entries_.find(dst));
		if ( entry_iter == entries_.end() )
		{
			FRL_THROW_S_CLASS( DstNotFound );
		}
		return entry_iter->second;
	}

	std::list<EntryPtr> EntriesMgr::GetEntriesBySrc( const string_t& src_name )
	{
		std::list<EntryPtr> entry_list;
		
		BOOST_AUTO(src_assoc_iter, src_assoc_.equal_range(src_name));
		BOOST_AUTO(entry_iter, src_assoc_iter.first);
		for (; entry_iter != src_assoc_iter.second; ++entry_iter)
		{
			entry_list.push_back(entry_iter->second);
		}
		return entry_list;
	}

	bool EntriesMgr::DstExist( const string_t& dst_name ) const
	{
		string_t dst = dst_name;
		if ( boost::contains( dst_name, "_CMD" ) )
		{
			dst = boost::replace_last_copy(dst, "_CMD", "_TS");
		}

		BOOST_AUTO(entry_iter, entries_.find(dst));
		if ( entry_iter == entries_.end() )
		{
			return false;
		}

		return true;
	}

	bool EntriesMgr::SrcExist( const string_t& src_name ) const
	{
		BOOST_AUTO(entry_iter, src_assoc_.find(src_name));
		if ( entry_iter == src_assoc_.end() )
		{
			return false;
		}

		return true;
	}

}