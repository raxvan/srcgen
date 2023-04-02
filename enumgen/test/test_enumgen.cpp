
#include <ttf.h>
#include "out/test1_decl.h"
#include "out/test1_impl.h"

#include "out/test2_decl.h"
#include "out/test2_impl.h"
#include "out/test2_struct.h"

void test_all_visit()
{

	using namespace test_enum;
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

bool handle_value(uint32_t& c, int v)
{
	c++;
	TEST_ASSERT(c == 1);
	TEST_ASSERT(v == 1);
	return false;
}
bool handle_value(uint32_t& c, float v)
{
	c++;
	TEST_ASSERT(c == 2);
	TEST_ASSERT(v == 2.0f);
	return true;
}

void test_generated_struct()
{
	using t = gs::StructUtils<test_struct::TestStruct2>;

	static_assert(sizeof(test_struct::TestStruct2) == sizeof(int) * test_enum::TestEnum2::kCount, "Error");
	static_assert(test_struct::TestStruct2::key() == test_enum::TestEnum2::key(), "Error");

	t		 tmp;
	uint32_t c = 0;
	tmp.long_visit<test_enum::TestEnum2::enum_member1, test_enum::TestEnum2::enum_member2>([&](const auto& m) {
		handle_value(c, m);
	});
	TEST_ASSERT(c == 2);

	c = 0;
	tmp.long_visit<test_enum::TestEnum2::enum_member1>([&](const auto& m) {
		handle_value(c, m);
	});
	TEST_ASSERT(c == 1);

	c = 0;
	tmp.short_visit<test_enum::TestEnum2::enum_member1, test_enum::TestEnum2::enum_member2>([&](const auto& m) { 
		return handle_value(c, m);
	});
	TEST_ASSERT(c == 1);
}

void test_speed()
{
	using namespace test_enum;
	using t = gs::EnumUtils<TestEnum>;
	std::size_t c = 0;
	std::size_t total = 1024;
	for (std::size_t i = 0; i < total; i++)
	{
		bool b = true;
		t::iterate([&](t::Enum e) { b = b && t::parse(t::name(e)) == e; });
		if (b)
			c++;
	}

	TEST_ASSERT(c == total);
}

void test_main()
{
	TEST_FUNCTION(test_all_visit);
	TEST_FUNCTION(test_generated_struct);

	TEST_FUNCTION(test_speed);
}
TEST_MAIN(test_main)
