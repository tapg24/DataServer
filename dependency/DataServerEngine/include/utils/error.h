#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace Error
{
	enum EnumErrorCode
	{
		ChannelNoError = 0,
		ChannelNotCreated,
		ChannelNotInited,
		ChannelNotStarted,
		ChannelNotConnected,
		ChannelCommonError
	};
	
	class ErrorCode
	{
		EnumErrorCode error_id;
		std::string error_message;
	
	public:
		ErrorCode();
		ErrorCode(const EnumErrorCode error_code, const std::string error_msg);
		~ErrorCode();

		EnumErrorCode GetId() const;
		std::string GetMessage() const;	
	};
}

#endif // ERROR_H