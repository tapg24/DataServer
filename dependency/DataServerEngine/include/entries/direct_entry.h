#ifndef direct_entry_h__
#define direct_entry_h__

#include "entries/entry.h"
#include "utils/string.h"
#include "utils/variant.h"

#include "../../jsoncpp-0.5.0/include/json/value.h"

namespace Channels
{
	class DirectEntry : public Channels::Entry
	{
	public:
		DirectEntry(const string_t& name, const string_t& src);
		~DirectEntry();

		// Entry interface
		void Update(const string_t& tag_name);
		bool IsChanged() const;
		std::list<string_t> GetSrcList() const;		

	private:
		string_t src_name_;
	};

	typedef boost::shared_ptr<DirectEntry> DirectEntryPtr;

}

#endif // direct_entry_h__
