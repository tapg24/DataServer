#ifndef string_h__
#define string_h__

#include <string>

#ifdef _UNICODE
	#define _STR(str) L##str
	typedef std::wstring string_t;
#else
	#define _STR(str) str
	typedef std::string string_t;
#endif 

#endif // string_h__
