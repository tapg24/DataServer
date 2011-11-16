#include <string>
#include <wtypes.h>

// Converting string to wstring
std::wstring widen( const std::string& in, std::locale loc  = std::locale() );

// Converting wstring to string
std::string narrow( const std::wstring& in, std::locale loc = std::locale() );

// Converting opc quality to string
std::string OPCQualityAsString(WORD quality);

// Converting FILETIME to string
std::string FiletimeAsString( const FILETIME &ft );
