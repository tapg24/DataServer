#ifndef json_variant_helper_h__
#define json_variant_helper_h__

#include "utils/variant.h"
#include "../../jsoncpp-0.5.0/include/json/forwards.h"

struct JsonHelper
{
	static void copy(const ComVariant& from, Json::Value& to) throw (std::runtime_error);

	static ComVariant GetComVariant(const Json::Value& value, const VARTYPE type) throw (std::runtime_error);
	static ComVariant GetComVariant(const Json::Value& value, const Json::Value& type) throw (std::runtime_error);
};

#endif // json_variant_helper_h__
