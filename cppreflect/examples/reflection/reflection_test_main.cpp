
#include "helpers/test_type1.h"
#include "helpers/test_type2.h"
#include "helpers/test_type3.h"
#include "helpers/test_type4.h"
#include <iostream>
#include <vector>

void test_type_visit()
{
	struct Visiter : public sg::InstanceDescriptor::ITypeVisiter
	{
		std::vector<sg::compiletime_identifier> data;
		virtual void							operator()(const sg::compiletime_identifier& i) override
		{
			data.push_back(i);
		}
	};
	test_type4 t;
	Visiter	   v;
	t.visit_types(v);

	auto f = [&](const sg::compiletime_identifier& i) {
		auto itr = std::find(v.data.begin(), v.data.end(), i);
		return itr != v.data.end();
	};

	TTF_ASSERT(f(sg::class_identifier_from<test_type1>::value()) == false);
	TTF_ASSERT(f(sg::class_identifier_from<test_type3>::value()) == false);
	TTF_ASSERT(f(sg::class_identifier_from<test_type2>::value()) == true);
	TTF_ASSERT(f(sg::class_identifier_from<test_type4>::value()) == true);
}

template <class T>
void validate_is_instance(const base_type* ct, base_type* t)
{
	TTF_ASSERT(ct->isinstance<T>() && ct->cast<T>() != nullptr);
	TTF_ASSERT(t->isinstance<T>() && t->cast<T>() != nullptr);
}
template <class T>
void validate_is_not(const base_type* ct, base_type* t)
{
	TTF_ASSERT(ct->isinstance<T>() == false);
	TTF_ASSERT(t->isinstance<T>() == false);
}

void test_instance_decl()
{
	base_type*		 i = nullptr;
	const base_type* ci = nullptr;

	TEST_INLINE() = [&]() {
		test_type1 t;
		i = &t;
		ci = &t;
		validate_is_instance<test_type1>(ci, i);

		validate_is_not<test_type2>(ci, i);
		validate_is_not<test_type3>(ci, i);
		validate_is_not<test_type4>(ci, i);
	};

	TEST_INLINE() = [&]() {
		test_type2 t;
		i = &t;
		ci = &t;
		validate_is_instance<test_type2>(ci, i);

		validate_is_not<test_type1>(ci, i);
		validate_is_not<test_type3>(ci, i);
		validate_is_not<test_type4>(ci, i);
	};

	TEST_INLINE() = [&]() {
		test_type3 t;
		i = &t;
		ci = &t;
		validate_is_instance<test_type3>(ci, i);

		validate_is_not<test_type1>(ci, i);
		validate_is_not<test_type2>(ci, i);
		validate_is_not<test_type4>(ci, i);
	};

	TEST_INLINE() = [&]() {
		test_type4 t;
		i = &t;
		ci = &t;
		validate_is_instance<test_type2>(ci, i);
		validate_is_instance<test_type4>(ci, i);

		validate_is_not<test_type1>(ci, i);
		validate_is_not<test_type3>(ci, i);
	};
}

void test_compiletime_identifier()
{
	constexpr sg::compiletime_identifier a = sg::compiletime_identifier::create("hello1");
	constexpr sg::compiletime_identifier b = sg::compiletime_identifier::create("hello2_", 6); // this could cause some problems
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
	TEST_FUNCTION(test_compiletime_identifier);
	TEST_FUNCTION(test_instance_decl);
	TEST_FUNCTION(test_type_visit);
}

TEST_MAIN(test_main)