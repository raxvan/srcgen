
#include <ttf.h>
#include "out/test1_decl.h"
#include "out/test1_impl.h"

using namespace test_enum;

void test_all_visit()
{
	using t = gs::EnumUtils<TestEnum>;

	uint32_t index = 0;
	t::iterate([&](t::Enum e) {
		TEST_ASSERT(t::parse(t::name(e)) == e);
		TEST_ASSERT(index == t::index(e));
		TEST_ASSERT(e == t::parse(index));
		index++;
		});
}

void test_main()
{
	TEST_FUNCTION(test_all_visit);

}
TEST_MAIN(test_main)
