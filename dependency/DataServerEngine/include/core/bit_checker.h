#ifndef bit_checker_h__
#define bit_checker_h__

#include "core/state_checker.h"
#include <list>
#include "utils/string.h"
#include "utils/types.h"

class BitChecker : public StateChecker
{
public:
	BitChecker(const string_t& stateName, std::list<int8_t> const set, std::list<int8_t> const unset);
	~BitChecker();

	bool Check(int32_t const number);

protected:
private:
	std::list<int8_t> set_;
	std::list<int8_t> unset_;
};

typedef boost::shared_ptr<BitChecker> BitCheckerPtr;

#endif // bit_checker_h__
