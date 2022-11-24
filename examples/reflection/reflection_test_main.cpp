
#include "example1.h"
#include <iostream>

class base : public sg::InstanceDescriptor
{
};

class type1 : public base
{
	DEFAULT_CLASS_NAME();
};
class type2 : public base
{
	CUSTOM_CLASS_NAME(custom_name);
};

class type3 : public type2
{
	CUSTOM_CLASS_NAME_EX(custom_name, type2);
};

void test_instance_decl()
{
	base* i = nullptr;
	const base* ci = nullptr;

	TEST_INLINE() = [&]() {
		type1 t;
		i = &t;
		ci = &t;
		TTF_ASSERT(i->isinstance<type1>() && i->cast<type1>() != nullptr);
		TTF_ASSERT(ci->isinstance<type1>() && ci->cast<type1>() != nullptr);

		TTF_ASSERT(i->isinstance<type2>() == false);
		TTF_ASSERT(ci->isinstance<type2>() == false);
	};

	TEST_INLINE() = [&]() {
		type2 t;
		i = &t;
		ci = &t;
		TTF_ASSERT(i->isinstance<type2>() && i->cast<type2>() != nullptr);
		TTF_ASSERT(ci->isinstance<type2>() && ci->cast<type2>() != nullptr);

		TTF_ASSERT(i->isinstance<type1>() == false);
		TTF_ASSERT(ci->isinstance<type1>() == false);
	};

	TEST_INLINE() = [&]() {
		type3 t;
		i = &t;
		ci = &t;
		TTF_ASSERT(i->isinstance<type3>() && i->cast<type3>() != nullptr);
		TTF_ASSERT(ci->isinstance<type3>() && ci->cast<type3>() != nullptr);

		TTF_ASSERT(i->isinstance<type2>() && i->cast<type2>() != nullptr);
		TTF_ASSERT(ci->isinstance<type2>() && ci->cast<type2>() != nullptr);

		TTF_ASSERT(i->isinstance<type1>() == false);
		TTF_ASSERT(ci->isinstance<type1>() == false);
	};
}

void test_compiletime_identifier()
{
	constexpr sg::compiletime_identifier a = sg::compiletime_identifier::create("hello1");
	constexpr sg::compiletime_identifier b = sg::compiletime_identifier::create("hello2_", 6); //this could cause some problems
	constexpr sg::compiletime_identifier c = "hello3"_id;

	static_assert(a.size == 6, "...");
	static_assert(b.size == 6, "...");
	static_assert(c.size == 6, "...");

	std::cout << a.id << std::endl;
	std::cout << b.id << std::endl;
	std::cout << c.id << std::endl;
}

void test_main()
{
	TEST_FUNCTION(test_instance_decl);
	TEST_FUNCTION(test_compiletime_identifier);
}

TEST_MAIN(test_main)