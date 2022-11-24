#pragma once

#pragma once

#include "reflection_config.h"

#ifdef __cpp_lib_source_location
#	include <source_location>
#	include <string_view>
#endif

namespace sg
{

	struct compiletime_identifier
	{
	public:
		const uint32_t hash = 0;
		const uint32_t size = 0; // name size
		const char* const id = nullptr;

	public:
		inline bool operator==(const compiletime_identifier& other) const
		{
			return hash == other.hash && size == other.size && id == other.id;
		}
		inline bool operator!=(const compiletime_identifier& other) const
		{
			return hash != other.hash || size != other.size || id != other.id;
		}
		inline bool operator<(const compiletime_identifier& other) const
		{
			if (hash != other.hash)
				return hash < other.hash;

			if (size != other.size)
				return size < other.size;

			return id < other.id;
		}

	public:
		compiletime_identifier() = default;
		compiletime_identifier(const compiletime_identifier&) = default;
		compiletime_identifier& operator=(const compiletime_identifier&) = default;

	private:
		constexpr compiletime_identifier(const uint32_t h, const uint32_t s, const char* i)
			: hash(h)
			, size(s)
			, id(i)
		{
		}

	public:
		static constexpr compiletime_identifier create(const char* str, std::size_t size) noexcept
		{
			/* http://www.isthe.com/chongo/tech/comp/fnv/ */
			uint32_t hash = 2166136261u;
			for (std::size_t i = 0; i < size; i++)
			{
				hash = hash ^ uint32_t(str[i]);
				hash = hash * 16777619;
			}
			return compiletime_identifier(hash, uint32_t(size), str);
		}
		static constexpr compiletime_identifier create(const char* str) noexcept
		{
			/* http://www.isthe.com/chongo/tech/comp/fnv/ */
			uint32_t	hash = 2166136261u;
			uint32_t	size = 0;
			const char* itr = str;
			while (*itr != '\0')
			{
				hash = hash ^ uint32_t(*itr++);
				hash = hash * 16777619;
				size++;
			}
			return compiletime_identifier(hash, size, str);
		}

#ifdef __cpp_lib_source_location
		static constexpr compiletime_identifier create(const std::source_location& location) noexcept
		{
			return create(location.function_name());
		}
#endif
	};

}

constexpr sg::compiletime_identifier operator"" _id(const char* str, std::size_t size)
{
	return sg::compiletime_identifier::create(str, size);
}
