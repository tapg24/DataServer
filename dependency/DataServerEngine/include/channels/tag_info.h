#ifndef TAG_INFO_H
#define TAG_INFO_H

#include <string>
#include <vector>
#include <wtypes.h>

namespace Channels
{
	struct TagInfo
	{
		TagInfo(const std::string& name, const VARTYPE& type, const float factor = 1);
		std::string name_;
		VARTYPE type_;
		float factor_;
	};

	typedef std::vector<TagInfo> TagInfoArray;
}

#endif // TAG_INFO_H