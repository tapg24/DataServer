#include "entries/null_entry.h"

namespace Channels
{

	NullEntry::NullEntry( const string_t& name )
		: Entry(name, NullEntry_Type)
	{
	}

	void NullEntry::Update( const string_t& tag_name )
	{

	}

	bool NullEntry::IsChanged() const
	{
		return false;
	}

	std::list<string_t> NullEntry::GetSrcList() const
	{
		return std::list<string_t>();
	}

}