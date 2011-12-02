#include "utils/json_variant_helper.h"
#include "../../jsoncpp-0.5.0/include/json/value.h"
#include <boost/format.hpp>

void JsonHelper::copy( const ComVariant& from, Json::Value& to ) throw (std::runtime_error)
{
	switch (from.getType())
	{
	case VT_R4: // float
		to = from.AsFloat();
		break;
	case VT_BSTR: // string
		to = from.AsString();
		break;
	case VT_BOOL: // bool
		to = from.AsBool();
		break;
	case VT_UI2: // word
		to = from.AsUShort();
		break;
	default:
		//throw std::runtime_error(str( boost::format("type not exists. index - %1% name - %2% type - %3%") % idx % name % type ));
		break;
	}
}

ComVariant JsonHelper::GetComVariant( const Json::Value& value, const VARTYPE type ) throw (std::runtime_error)
{
	ComVariant ret;
	switch (type)
	{
	case 4: // float
		ret = static_cast<float>(value.asDouble());
		break;
	case 8: // string
		ret = value.asString();
		break;
	case 11: // bool
		ret = value.asBool();
		break;
	case 18: // word VT_UI2
		ret = static_cast<WORD>(value.asUInt());
		break;
	default:
		throw std::runtime_error(str( boost::format("type not exists. type - %1%") % type ));
		break;
	}

	return ret;
}

ComVariant JsonHelper::GetComVariant( const Json::Value& value, const Json::Value& type ) throw (std::runtime_error)
{
	return GetComVariant(value, type.asInt()).getConstRef();
}