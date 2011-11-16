#include "entries/ts_def.h"
#include <bitset>
#include <boost/typeof/typeof.hpp>
#include <boost/format.hpp>

// TS STATES
std::map<string_t, uint16_t> TS_STATE_HOLDER::map;

namespace
{
	TS_STATE_HOLDER __force_state_holder__;
}

TS_STATE_HOLDER::TS_STATE_HOLDER()
{
	map["MANUAL"] = TS_STATE::MANUAL;
	map["AUTO"] = TS_STATE::AUTO;
	map["CHANGED"] = TS_STATE::CHANGED;
	map["ACKN_ED"] = TS_STATE::ACKN_ED;
	map["BAD"] = TS_STATE::BAD;
	map["RACKED"] = TS_STATE::RACKED;
	map["AMBIGUITY"] = TS_STATE::AMBIGUITY;
	map["OFF"] = TS_STATE::OFF;
	map["ON"] = TS_STATE::ON;
}

std::list<string_t> TS_STATE_HOLDER::GET_KEYS()
{
	std::list<string_t> keys;

	BOOST_AUTO( iter, map.begin() );
	BOOST_AUTO( end, map.end() );
	for(; iter != end; ++iter)
	{
		keys.push_back(iter->first);
	}

	return keys;
}

uint16_t TS_STATE_HOLDER::GET_VALUE( const string_t& key )
{
	BOOST_AUTO(where, map.find(key));
	if ( where == map.end() )
	{
		FRL_THROW_CLASS( StateNotValid, key );
	}
	return where->second;
}

string_t TS_STATE_HOLDER::GET_VALUES_AS_STRING( uint64_t state )
{
	string_t result = boost::str(boost::format("[dec:%1%][bin:%2%]") % state % std::bitset<16>(state));

	BOOST_AUTO( iter, map.begin() );
	BOOST_AUTO( end, map.end() );
	for(; iter != end; ++iter)
	{
		if ( state & iter->second )
		{
			result += "[" + iter->first + "]";
		}
	}

	return result;
}

// TS COMMANDS
std::map<string_t, uint16_t> TS_CMD_HOLDER::map;

namespace
{
	TS_CMD_HOLDER __force_cmd_holder__;
}

TS_CMD_HOLDER::TS_CMD_HOLDER()
{
	map["SET_MANUAL"] = TS_CMD::SET_MANUAL;
	map["SET_AUTO"] = TS_CMD::SET_AUTO;
	//map["SET_CHANGED"] = TS_CMD::SET_CHANGED;
	map["SET_ACKN_ED"] = TS_CMD::SET_ACKN_ED;
	//map["SET_BAD"] = TS_CMD::SET_BAD;
	map["SET_RACKED"] = TS_CMD::SET_RACKED;
	map["SET_AMBIGUITY"] = TS_CMD::SET_AMBIGUITY;
	map["SET_OFF"] = TS_CMD::SET_OFF;
	map["SET_ON"] = TS_CMD::SET_ON;
}

std::list<string_t> TS_CMD_HOLDER::GET_KEYS()
{
	std::list<string_t> keys;

	BOOST_AUTO( iter, map.begin() );
	BOOST_AUTO( end, map.end() );
	for(; iter != end; ++iter)
	{
		keys.push_back(iter->first);
	}

	return keys;
}

uint16_t TS_CMD_HOLDER::GET_VALUE( const string_t& key )
{
	BOOST_AUTO(where, map.find(key));
	if ( where == map.end() )
	{
		FRL_THROW_CLASS( CmdNotValid, key );
	}
	return where->second;
}
