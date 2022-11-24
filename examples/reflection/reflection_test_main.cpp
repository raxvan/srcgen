
#include "example1.h"

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

void test_main()
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


TEST_MAIN(test_main)