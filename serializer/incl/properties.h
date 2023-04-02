
#pragma once

#include "data_iterator.h"
#include <generated_source.h>

namespace ss
{
	struct _detail
	{
		static constexpr std::size_t struct_header_size()
		{
			return 8;
		}
		static constexpr std::size_t priperty_header_size()
		{
			return 4;
		}
		static constexpr std::size_t int_size()
		{
			return 4;
		}
		static constexpr std::size_t unsigned_size()
		{
			return 4;
		}
		static constexpr std::size_t float_size()
		{
			return 4;
		}

		static data_iterator write_header_struct(const uint32_t sid, byte_t* itr, const uint32_t data_size);
		static bool parse_struct_header(const uint32_t sid, data_iterator& itr);
	};

	//--------------------------------------------------------------------------------------------------------------------------------

	struct IntProperty
	{
	public:
		int32_t value = 0;

	public:
		IntProperty() = default;
		IntProperty(const int32_t v)
			:value(v)
		{}

	public:
		inline constexpr std::size_t binsize() const
		{
			return _detail::priperty_header_size() + _detail::int_size();
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);

#ifdef SERIALIZER_TESTING
		inline bool equals(const IntProperty& other) const
		{
			return value == other.value;
		}
#endif
	};

	struct UnsignedProperty
	{
	public:
		uint32_t value = 0;

	public:
		UnsignedProperty() = default;
		UnsignedProperty(const uint32_t v)
			:value(v)
		{}

	public:
		inline constexpr std::size_t binsize() const
		{
			return _detail::priperty_header_size() + _detail::unsigned_size();
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);
#ifdef SERIALIZER_TESTING
		inline bool equals(const UnsignedProperty& other) const
		{
			return value == other.value;
		}
#endif
	};


	//--------------------------------------------------------------------------------------------------------------------------------

	struct FloatProperty
	{
	public:
		float value = 0.0f;

	public:
		FloatProperty() = default;
		FloatProperty(const float v)
			:value(v)
		{}

	public:
		inline constexpr std::size_t binsize() const
		{
			return _detail::priperty_header_size() + _detail::float_size();
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);

#ifdef SERIALIZER_TESTING
		inline bool equals (const FloatProperty& other) const
		{
			return value == other.value;
		}
#endif
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	template <class T>
	struct BaseArray
	{
	public:
		T* values = nullptr;
		uint32_t count = 0;
#ifdef SERIALIZER_TESTING
		inline bool equals(const BaseArray<T>& other) const
		{
			if (count != other.count)
				return false;

			for (std::size_t i = 0; i < count; i++)
				if (values[i] != other.values[i])
					return false;
			return true;
		}
#endif

		inline T& operator [](const std::size_t index)
		{
			SERIALIZER_ASSERT(values != nullptr && index < count);
			return values[index];
		}
		inline const T& operator [](const std::size_t index) const
		{
			SERIALIZER_ASSERT(values != nullptr && index < count);
			return values[index];
		}
	};

	struct IntArrayProperty : public BaseArray<int32_t>
	{
	public:
		inline std::size_t binsize() const
		{
			return
				_detail::priperty_header_size() + 
				_detail::unsigned_size() +
				_detail::int_size() * count;
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);
	};

	struct FloatArrayProperty : public BaseArray<float>
	{
	public:
		inline std::size_t binsize() const
		{
			return
				_detail::priperty_header_size() +
				_detail::unsigned_size() +
				_detail::float_size() * count;
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);
	};

	struct UnsignedArrayProperty : public BaseArray<uint32_t>
	{
	public:
		inline std::size_t binsize() const
		{
			return
				_detail::priperty_header_size() +
				_detail::unsigned_size() +
				_detail::unsigned_size() * count;
		}
		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);
	};

	//--------------------------------------------------------------------------------------------------------------------------------

	struct StringProperty
	{
	public:
		const char* cstr = "";
		uint32_t length = 0;
	public:
		StringProperty() = default;

		template <std::size_t N>
		inline void operator = (const char (&_value)[N])
		{
			static_assert(N < (std::numeric_limits<uint32_t>::max() - 1), "String too long");
			cstr = _value;
			length = N;
		}
		inline void operator = (const std::string_view& sv)
		{
			SERIALIZER_ASSERT(sv.size() < (std::numeric_limits<uint32_t>::max() - 1));
			cstr = sv.data();
			length = (uint32_t)sv.size();
		}
		inline std::string_view get() const
		{
			return { cstr, length };
		}
	public:
		std::size_t binsize() const;

		void write(const uint32_t pid, data_iterator& itr) const;
		void parse(const uint32_t pid, data_iterator& itr);

#ifdef SERIALIZER_TESTING
		inline bool equals(const StringProperty& other) const
		{
			if(length != other.length)
				return false;
			for(std::size_t i = 0; i < length;i++)
				if(cstr[i] != other.cstr[i])
					return false;
			return true;
		}
#endif
	};

};
