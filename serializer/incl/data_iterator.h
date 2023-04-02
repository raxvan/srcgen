
#pragma once

#include "serializer_config.h"

namespace ss
{

	//--------------------------------------------------------------------------------------------------------------------------------

	struct data_iterator
	{
	public:
		bool accept(uint32_t ID);
		byte_t* get(const std::size_t sz);

		inline uint32_t id() const
		{
			SERIALIZER_ASSERT(start != nullptr && (start + sizeof(uint32_t)) <= end);
			return *reinterpret_cast<uint32_t*>(start);
		}

		inline bool hasdata() const
		{
			return start < end;
		}

	public:
		template <class T>
		inline void push_value(const uint32_t pid, const T& value)
		{
			SERIALIZER_ASSERT(start != nullptr && (start + sizeof(uint32_t) + sizeof(T)) <= end);
			*reinterpret_cast<uint32_t*>(start) = pid;
			start += sizeof(uint32_t);
			*reinterpret_cast<T*>(start) = value;
			start += sizeof(T);
		}
		template <class T>
		inline T* pop_value(const uint32_t pid)
		{
			SERIALIZER_ASSERT(start != nullptr && (start + sizeof(uint32_t) + sizeof(T)) <= end);
			if(accept(pid))
			{
				auto r = reinterpret_cast<T*>(start);
				start += sizeof(T);
				return r;
			}
			return nullptr;
		}
	public:
		byte_t* start = nullptr;
		const byte_t* end = nullptr;
	};


};
