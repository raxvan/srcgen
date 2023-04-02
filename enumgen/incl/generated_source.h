
#pragma once

#include <cstring>
#include <string_view>

namespace gs
{

	struct utils
	{

		static inline uint32_t simple_string_hash(const char* s, const std::size_t sz)
		{
			// same as cpp_generate.py hash func
			uint32_t hash_value = 1423;
			for (std::size_t i = 0; i < sz; i++)
				hash_value = (hash_value << 4) + uint32_t(*s++);
			return hash_value;
		}
	};

	template <class B>
	struct EnumUtils : public B
	{
	public:
		using base_enum_t = B::Enum;

	public:
		using B::parse;
		inline static base_enum_t parse(const uint32_t ev)
		{
			if (ev < base_enum_t::kCount)
				return base_enum_t(ev);
			return base_enum_t::kCount;
		}
		inline static base_enum_t parse(const std::string_view& sv)
		{
			return B::parse(sv.data(), sv.size());
		}

	public:
		inline static bool valid(const uint32_t ev)
		{
			return ev < base_enum_t::kCount;
		}
		inline static bool valid(const base_enum_t ev)
		{
			return ev != base_enum_t::kCount;
		}
		inline static uint32_t index(const base_enum_t ev)
		{
			return uint32_t(ev);
		}
		template <base_enum_t evalue>
		constexpr static uint32_t index()
		{
			static_assert(evalue != base_enum_t::kCount, "Count is not allowed to be used as index!");
			return uint32_t(evalue);
		}

	public:
		using B::name;
		inline static std::string_view name(const uint32_t index)
		{
			return name(parse(index));
		}

		template <class F>
		static void iterate(const F& _func)
		{
			for (uint32_t i = 0; i < base_enum_t::kCount; i++)
				_func(parse(i));
		}
	};

	template <class S>
	struct StructUtils : public S
	{
	public:
		using base_struct_t = S;
	};

};
