#include "test1_decl.h"
namespace test_enum
{
	TestEnum::Enum TestEnum::parse(const char* _name, const std::size_t nmsize)
	{
		if (_name == nullptr)
			return TestEnum::Enum::kCount;
		switch (nmsize)
		{
			case 11:
			{
				if (std::strncmp(_name, "uEnumValueB", 11) == 0)
					return Enum::kEnumValueB;
				if (std::strncmp(_name, "uEnumValueA", 11) == 0)
					return Enum::kEnumValueA;
				break;
			}
			case 12:
			{
				if (std::strncmp(_name, "uEnumValueAB", 12) == 0)
					return Enum::kEnumValueAB;
				break;
			}
			default:break;
		}
		return TestEnum::Enum::kCount;
	}
	std::string_view TestEnum::name(const TestEnum::Enum v)
	{
		switch(v)
		{
			case Enum::kEnumValueA  : return std::string_view("uEnumValueA", 11);
			case Enum::kEnumValueB  : return std::string_view("uEnumValueB", 11);
			case Enum::kEnumValueAB : return std::string_view("uEnumValueAB", 12);
			default:break;
		}
		return {};
	}
}
