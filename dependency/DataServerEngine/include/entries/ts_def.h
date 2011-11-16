#ifndef ts_def_h__
#define ts_def_h__

#include "utils/types.h"
#include "utils/string.h"
#include "../../FRL/include/frl_exception.h"
#include <map>
#include <list>

// TS STATES
namespace TS_STATE
{
	const uint16_t MANUAL = 0x8000; // 32768;
	const uint16_t AUTO = 0x4000; // 16384;
	const uint16_t CHANGED = 0x2000; // 8192;
	const uint16_t ACKN_ED = 0x1000; // 4096;
	const uint16_t BAD = 0x10; // 16;
	const uint16_t RACKED = 0x8; // 8;
	const uint16_t AMBIGUITY = 0x4; // 4;
	const uint16_t OFF = 0x2; // 2;
	const uint16_t ON = 0x1; // 1;
}

struct TS_STATE_HOLDER
{
	FRL_EXCEPTION_CLASS( StateNotValid );

	TS_STATE_HOLDER();

	static std::list<string_t> GET_KEYS();

	static uint16_t GET_VALUE(const string_t& key);

	static string_t GET_VALUES_AS_STRING(uint64_t state);

private:
	static std::map<string_t, uint16_t> map;
};

// TS COMMANDS
namespace TS_CMD
{
	const uint16_t SET_MANUAL = 0x8000; // 32768;
	const uint16_t SET_AUTO = 0x4000; // 16384;
	//const uint16_t SET_CHANGED = 0x2000; // 8192;
	const uint16_t SET_ACKN_ED = 0x1000; // 4096;
	//const uint16_t SET_BAD = 0x10; // 16;
	const uint16_t SET_RACKED = 0x8; // 8;
	const uint16_t SET_AMBIGUITY = 0x4; // 4;
	const uint16_t SET_OFF = 0x2; // 2;
	const uint16_t SET_ON = 0x1; // 1;
}

struct TS_CMD_HOLDER
{
	FRL_EXCEPTION_CLASS( CmdNotValid );

	TS_CMD_HOLDER();

	static std::list<string_t> GET_KEYS();

	static uint16_t GET_VALUE(const string_t& key);

private:
	static std::map<string_t, uint16_t> map;
};

#endif // ts_def_h__
