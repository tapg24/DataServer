#include "core/state_checker.h"


StateChecker::StateChecker( string_t const name ) :
	stateName_(name)
{
}

StateChecker::~StateChecker( )
{
}

string_t StateChecker::GetStateName() const
{
	return this->stateName_;
}
