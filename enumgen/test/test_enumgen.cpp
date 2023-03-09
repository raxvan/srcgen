
#include <ttf.h>
#include "out/test1_decl.h"
#include "out/test1_impl.h"

using namespace test_enum;

void test_all_visit()
{
	using t = gs::EnumUtils<TestEnum>;

	uint32_t index = 0;
	t::iterate([&](t::Enum e) {
		TEST_ASSERT(t::valid(e) == true);
		TEST_ASSERT(t::parse(t::name(e)) == e);
		TEST_ASSERT(index == t::index(e));
		TEST_ASSERT(e == t::parse(index));

		std::string en = std::string(t::name(e));
		TEST_ASSERT(en.size() > 1);
		en[0] = 'x';

		TEST_ASSERT(t::parse(en) == t::kCount);
		TEST_ASSERT(t::valid(t::parse(en)) == false);

		index++;
	});

}


void test_speed()
{
	using t = gs::EnumUtils<TestEnum>;
	std::size_t c = 0;
	std::size_t total = 1024;
	for (std::size_t i = 0; i < total; i++)
	{
		bool b = true;
		t::iterate([&](t::Enum e) {
			b = b && t::parse(t::name(e)) == e;
			});
		if (b)
			c++;
	}

	TEST_ASSERT(c == total);
}

void test_main()
{
	TEST_FUNCTION(test_all_visit);
	TEST_FUNCTION(test_speed);

}
TEST_MAIN(test_main)
