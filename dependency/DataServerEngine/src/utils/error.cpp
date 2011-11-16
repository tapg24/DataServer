#include "utils/error.h"

namespace Error
{
	ErrorCode::ErrorCode()
	{

	}

	ErrorCode::ErrorCode( const EnumErrorCode error_code, const std::string error_msg ) : error_id(error_code), error_message(error_msg)
	{
	
	}

	ErrorCode::~ErrorCode()
	{
	
	}

	EnumErrorCode ErrorCode::GetId() const
	{
		return error_id;
	}

	std::string ErrorCode::GetMessage() const
	{
		return error_message;
	}

}
