#include "core/bit_checker.h"
#include <boost/foreach.hpp>

bool CheckBit(int32_t const number, int8_t const bit)
{
	return !!( number & ( 1 << bit ) );
}

BitChecker::BitChecker( const string_t& stateName, std::list<int8_t> const set, std::list<int8_t> const unset ) :
	StateChecker(stateName), set_(set), unset_(unset)
{

}

BitChecker::~BitChecker()
{

}

bool BitChecker::Check( int32_t const number )
{
	BOOST_FOREACH( int8_t const bit, set_ )
	{
		if ( !CheckBit(number, bit) ) return false;
	}

	BOOST_FOREACH( int8_t bit, unset_ )
	{
		if ( CheckBit(number, bit) ) return false;
	}

	return true;
}
