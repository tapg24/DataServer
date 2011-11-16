#ifndef state_checker_h__
#define state_checker_h__

#include "utils/string.h"
#include "utils/types.h"
#include <boost/shared_ptr.hpp>

class StateChecker
{
public:
	StateChecker(string_t const name);
	virtual ~StateChecker();

	string_t GetStateName() const;

	virtual bool Check(int32_t const number) = 0;

private:
	string_t stateName_;
};

typedef boost::shared_ptr<StateChecker> StateCheckerPtr;

#endif // state_checker_h__
