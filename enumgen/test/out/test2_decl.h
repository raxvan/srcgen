#pragma once
/*
Autogenerated from:
	../test2.autogen.py
*/
#include <generated_source.h>
namespace test_enum
{
	#define GENUM_TESTENUM2_KEY 3663250048
	struct TestEnum2
	{
	public:
		constexpr static uint32_t key() { return 3663250048; }
	public:
		enum Enum
		{
			enum_member1 = 0,
			enum_member2 = 1,
			kCount       = 2
		};
	public:
		static TestEnum2::Enum parse(const char* name, const std::size_t nmsize);
		static std::string_view name(const TestEnum2::Enum v);
	public:
		constexpr static uint32_t count()
		{
			return 2;
		}
	};
}