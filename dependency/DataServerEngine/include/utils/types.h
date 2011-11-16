#ifndef TYPES_H
#define TYPES_H

#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4996)

#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#define BOOST_PARAMETER_MAX_ARITY 8

//////////////////////////////////////////////////////////////////////////
#include <boost/cstdint.hpp>
using boost::int64_t;
using boost::uint64_t;
using boost::int32_t;
using boost::uint32_t;
using boost::int16_t;
using boost::uint16_t;
using boost::int8_t;
using boost::uint8_t;
//////////////////////////////////////////////////////////////////////////

// util types

struct EmptyType
{};

template <int v>
struct Int2Type
{
  enum
  {
	  value = v
  };
};

template <class Type>
struct Type2Type
{
	typedef Type OriginalType;
};

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef unsigned short USHORT;

//////////////////////////////////////////////////////////////////////////
// move it from here!!!
//#include <string>
//#include <wtypes.h>
//struct TagType
//{
//	std::string name;
//	VARTYPE type;
//	float factor;
//};

#endif //TYPES_H