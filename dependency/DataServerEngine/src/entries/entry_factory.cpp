#include "entries/entry_factory.h"

#include "entries/null_entry.h"
#include "entries/ts_entry.h"
#include "entries/direct_entry.h"
#include "../../jsoncpp-0.5.0/include/json/value.h"
#include <boost/make_shared.hpp>
#include <boost/range/algorithm/count.hpp>

namespace Channels
{
	EntryPtr Channels::EntryFactory::Create(const string_t& name, const Json::Value& content)
	{
		Json::Value::Members members = content.getMemberNames();
		if ( boost::count(members, string_t("bit")) )
		{
			return boost::make_shared<EntryTS>(name, content["bit"]);
		}
		else if ( boost::count(members, string_t("direct")) )
		{
			return boost::make_shared<DirectEntry>(name, content["direct"].asString());
		}
		else
		{
			return boost::make_shared<NullEntry>(name);
		}
	}
}