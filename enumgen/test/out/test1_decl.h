#pragma once
#include <generated_source.h>
namespace test_enum
{
	struct TestEnum
	{
		enum Enum
		{
			kEnumValueA  = 0,
			kEnumValueB  = 1,
			kEnumValueAB = 2,
			kCount       = 3
		};
		static TestEnum::Enum parse(const char* name, const std::size_t nmsize);
		static std::string_view name(const TestEnum::Enum v);
	};
}
