
#pragma once

#include <cstring>
#include <string_view>

namespace gs
{

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
		inline static uint32_t index(const base_enum_t ev)
		{
			return uint32_t(ev);
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
			for(uint32_t i = 0; i < base_enum_t::kCount; i++)
				_func(parse(i));
		}
	};

};

