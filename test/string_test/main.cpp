#include "utils/string.h"

#define BOOST_TEST_MODULE string

#include <boost/test/included/unit_test.hpp>

namespace impl {
	string_t  get_string(const char* c_str)
	{
		return string_t(c_str);
	}
} // namespace impl

BOOST_AUTO_TEST_CASE(first) {
	BOOST_CHECK_EQUAL(impl::get_string("123"), "123");
}