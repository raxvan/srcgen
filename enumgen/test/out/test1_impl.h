#include "test1_decl.h"
namespace test_enum
{
	TestEnum::Enum TestEnum::parse(const char* _name, const std::size_t nmsize)
	{
		if (_name == nullptr)
			return TestEnum::Enum::kCount;
		switch (nmsize)
		{
			case 10:
			{
				if (std::strncmp(_name, "EnumValueB", 10) == 0)
					return Enum::kEnumValueB;
				if (std::strncmp(_name, "EnumValueA", 10) == 0)
					return Enum::kEnumValueA;
				break;
			}
			case 11:
			{
				if (std::strncmp(_name, "EnumValueAB", 11) == 0)
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
			case Enum::kEnumValueA  : return std::string_view("EnumValueA", 10);
			case Enum::kEnumValueB  : return std::string_view("EnumValueB", 10);
			case Enum::kEnumValueAB : return std::string_view("EnumValueAB", 11);
			default:break;
		}
		return {};
	}
}
