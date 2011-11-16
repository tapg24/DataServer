#include "entries/entry.h"

namespace Channels
{

	Entry::Entry( const string_t& name, EntryType type )
		: name_(name), type_(type)
	{
	}

	Entry::~Entry()
	{
	}

	string_t Entry::GetName() const
	{
		return name_;
	}

	Channels::EntryType Entry::GetType() const
	{
		return type_;
	}
}