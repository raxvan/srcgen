
#include <ttf.h>
#include "out/test1_decl.h"
#include "out/test1_impl.h"

using namespace test_enum;

void test_main()
{
	using t = gs::EnumUtils<TestEnum>;

	TEST_ASSERT(t::parse(t::name(t::kEnumValueA)) == t::kEnumValueA);
	TEST_ASSERT(t::parse(t::name(t::kEnumValueB)) == t::kEnumValueB);
	TEST_ASSERT(t::parse(t::name(t::kEnumValueAB)) == t::kEnumValueAB);
}
TEST_MAIN(test_main)