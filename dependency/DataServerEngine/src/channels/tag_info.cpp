#include "channels/tag_info.h"

namespace Channels
{
	TagInfo::TagInfo( const std::string& name, const VARTYPE& type, const float factor /*= 1*/ )
		: name_(name), type_(type), factor_(factor)
	{
	}
}