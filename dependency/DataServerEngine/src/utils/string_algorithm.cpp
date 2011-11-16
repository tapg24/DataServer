#include "utils/string_algorithm.h"
#include "../../opc_foundation/opcda.h"
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>

// Converting string to wstring
std::wstring widen( const std::string& in, std::locale loc /*  = std::locale() */ )
{
	std::wstring out( in.length(), 0 );
	std::string::const_iterator i = in.begin(), ie = in.end();
	std::wstring::iterator j = out.begin();

	for( ; i!=ie; ++i, ++j )
		*j = std::use_facet< std::ctype< wchar_t > > ( loc ).widen( *i );

	return out;
}

// Converting wstring to string
std::string narrow( const std::wstring& in, std::locale loc /*  = std::locale() */ )
{
	std::string out( in.length(), 0 );
	std::wstring::const_iterator i = in.begin(), ie = in.end();
	std::string::iterator j = out.begin();

	for( ; i!=ie; ++i, ++j )
		*j = std::use_facet< std::ctype< wchar_t > > ( loc ).narrow( *i );

	return out;
}

std::string OPCQualityAsString(WORD quality)
{
	if ( quality == OPC_QUALITY_BAD ) return "BAD";
	else if ( quality == OPC_QUALITY_UNCERTAIN ) return "UNCERTAIN";
	else if ( quality == OPC_QUALITY_GOOD ) return "GOOD";
	else
	{
		std::string out;
		if ( (quality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD )
		{
			if ( quality == OPC_QUALITY_CONFIG_ERROR ) out = "BAD - OPC_QUALITY_CONFIG_ERROR";
			if ( quality == OPC_QUALITY_NOT_CONNECTED ) out = "BAD - OPC_QUALITY_NOT_CONNECTED";
			if ( quality == OPC_QUALITY_DEVICE_FAILURE ) out = "BAD - OPC_QUALITY_DEVICE_FAILURE";
			if ( quality == OPC_QUALITY_SENSOR_FAILURE ) out = "BAD - OPC_QUALITY_SENSOR_FAILURE";
			if ( quality == OPC_QUALITY_LAST_KNOWN ) out = "BAD - OPC_QUALITY_LAST_KNOWN";
			if ( quality == OPC_QUALITY_COMM_FAILURE ) out = "BAD - OPC_QUALITY_COMM_FAILURE";
			if ( quality == OPC_QUALITY_OUT_OF_SERVICE ) out = "BAD - OPC_QUALITY_OUT_OF_SERVICE";
			if ( quality == OPC_QUALITY_WAITING_FOR_INITIAL_DATA ) out = "BAD - OPC_QUALITY_WAITING_FOR_INITIAL_DATA";
		}
		else if ( (quality & OPC_QUALITY_MASK) == OPC_QUALITY_UNCERTAIN )
		{
			if ( quality == OPC_QUALITY_LAST_USABLE ) out = "UNCERTAIN - OPC_QUALITY_LAST_USABLE";
			if ( quality == OPC_QUALITY_SENSOR_CAL ) out = "UNCERTAIN - OPC_QUALITY_SENSOR_CAL";
			if ( quality == OPC_QUALITY_EGU_EXCEEDED ) out = "UNCERTAIN - OPC_QUALITY_EGU_EXCEEDED";
			if ( quality == OPC_QUALITY_SUB_NORMAL ) out = "UNCERTAIN - OPC_QUALITY_SUB_NORMAL";
		}
		else if ( (quality & OPC_QUALITY_MASK) == OPC_QUALITY_GOOD )
		{
			if ( quality == OPC_QUALITY_LOCAL_OVERRIDE ) out = "OPC_QUALITY_GOOD - OPC_QUALITY_LOCAL_OVERRIDE";
		}

		if ( out.empty() )
		{
			out = "UNKNOWN QUALITY";
		}

		return out;
	}
}

std::string FiletimeAsString( const FILETIME &ft )
{
	SYSTEMTIME stUTC, st;
	FILETIME localFt;
	FileTimeToSystemTime (&ft, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &st);
	SystemTimeToFileTime(&st, &localFt);
	return  boost::posix_time::to_simple_string(boost::posix_time::from_ftime<boost::posix_time::ptime>(localFt));
}