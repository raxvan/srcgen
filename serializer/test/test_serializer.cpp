
#include <ttf.h>

#include <serializer.h>
#include "out/test1_decl.h"
#include "out/test1_impl.h"
#include "out/test1_struct.h"

#include <array>
using namespace test;


void test_writer()
{
	gs::StructUtils<test::TestStruct> sa;
	gs::StructUtils<test::TestStruct> sb;

	std::array<int,4> ia = {1,2,3,4};
	std::array<float,4> fa = {1.0f,2.0f,3.0f,4.0f};

	{
		sa.mint.value = 123;
		sa.mfloat.value = 123.0f;
		sa.mstring = "hello world";
		sa.mint_array.values = ia.data();
		sa.mint_array.count = 3; // <- intentional
		sa.mfloat_array.values = fa.data();
		sa.mfloat_array.count = 4;
	}

	std::vector<char> data;

	auto d = ss::struct_serializer<test::TestStruct>::write(sa, [&](const std::size_t sz) -> void* {
		data.resize(sz * 2);
		return data.data();
	});

	bool v = ss::struct_serializer<test::TestStruct>::parse(sb, d);
	TEST_ASSERT(v == true);

	{
		TEST_ASSERT(sa.mint.equals(sb.mint));
		TEST_ASSERT(sa.mfloat.equals(sb.mfloat));
		TEST_ASSERT(sa.mstring.equals(sb.mstring));
		TEST_ASSERT(sa.mint_array.equals(sb.mint_array));
		TEST_ASSERT(sa.mfloat_array.equals(sb.mfloat_array));
	}

}


void test_main()
{
	TEST_FUNCTION(test_writer);
}
TEST_MAIN(test_main)
