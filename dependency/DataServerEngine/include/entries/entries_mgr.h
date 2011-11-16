#ifndef entries_mgr_h__
#define entries_mgr_h__

#include "entries/entry.h"
#include "utils/string.h"
#include <list>
#include <map>
#include "../../FRL/include/frl_exception.h"

namespace Channels
{
	class EntriesMgr
	{
		std::map<string_t, EntryPtr> entries_; // dst -> Entry
		std::multimap<string_t, EntryPtr> src_assoc_; // src -> Entry

	public:
		FRL_EXCEPTION_CLASS( DstNotFound );
		FRL_EXCEPTION_CLASS( SrcNotFound );

		EntriesMgr();
		~EntriesMgr();

		void AddEntry(EntryPtr entry);
		bool DstExist(const string_t& dst_name) const;
		EntryPtr GetEntryByDst(const string_t& dst_name);
		bool SrcExist(const string_t& src_name) const;
		std::list<EntryPtr> GetEntriesBySrc(const string_t& src_name);
	};
}

#endif // entries_mgr_h__
