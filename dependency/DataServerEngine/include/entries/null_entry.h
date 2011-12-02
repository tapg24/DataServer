#ifndef null_entry_h__
#define null_entry_h__

#include "entries/entry.h"
#include "utils/string.h"

namespace Channels
{
	class NullEntry : public Channels::Entry
	{
	public:
		NullEntry(const string_t& name);
		~NullEntry() {};

		// Entry interface
		void Update(const string_t& tag_name);
		bool IsChanged() const;
		std::list<string_t> GetSrcList() const;		
	};

	typedef boost::shared_ptr<NullEntry> NullEntryPtr;

}

#endif // null_entry_h__
