#ifndef entry_factory_h__
#define entry_factory_h__

#include "utils/string.h"
#include "entries/entry.h"
#include "../../jsoncpp-0.5.0/include/json/forwards.h"
#include "../../FRL/include/frl_exception.h"

namespace Channels
{
	class EntryFactory
	{
		FRL_EXCEPTION_CLASS( TypeNotValid );

	public:
		static EntryPtr Create(const string_t& name, const Json::Value& content);
	};
}

#endif // entry_factory_h__
