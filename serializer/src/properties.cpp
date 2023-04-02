
#include <serializer.h>

namespace ss
{
	inline bool _is_valid_itr(const byte_t* itr)
	{
		return itr != nullptr && ((reinterpret_cast<uintptr_t>(itr) & 3) == 0);
	}
	inline bool _is_aligned(const byte_t* itr)
	{
		return ((reinterpret_cast<uintptr_t>(itr) & 3) == 0);
	}
	inline uint32_t _alignTo4(uint32_t v)
	{
		return (v + 3) & ~3;
	}

	data_iterator _detail::write_header_struct(const uint32_t sid, byte_t* itr, const uint32_t data_size)
	{
		SERIALIZER_ASSERT(_is_valid_itr(itr));
		data_iterator i;
		i.start = itr;
		i.end = itr + data_size + sizeof(uint32_t) * 2;

		i.push_value<uint32_t>(sid, data_size);
		return i;
	}

	bool _detail::parse_struct_header(const uint32_t sid, data_iterator& itr)
	{
		SERIALIZER_ASSERT(_is_valid_itr(itr.start));
		uint32_t* ds = itr.pop_value<uint32_t>(sid);
		if(ds != nullptr && (*ds) > 0)
			return (itr.start + (*ds)) == itr.end;
		return false;
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	void IntProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<int32_t>(pid, value);
	}
	void IntProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<int32_t>(pid);
		if(v != nullptr)
			value = *v;
	}

	//--------------------------------------------------------------------------------------------------------------------------------


	void UnsignedProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<uint32_t>(pid, value);
	}

	void UnsignedProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<uint32_t>(pid);
		if(v != nullptr)
			value = *v;
	}

	//--------------------------------------------------------------------------------------------------------------------------------


	void FloatProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<float>(pid, value);
	}

	void FloatProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<float>(pid);
		if(v != nullptr)
			value = *v;
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	void IntArrayProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<uint32_t>(pid, count);
		if(count > 0)
		{
			SERIALIZER_ASSERT(values != nullptr);
			auto sz = sizeof(int32_t) * count;
			std::memcpy(itr.get(sz), values, sz);
		}
	}

	void IntArrayProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<uint32_t>(pid);
		if(v == nullptr)
			return;

		count = *v;
		if(count > 0)
		{
			auto sz = sizeof(int32_t) * count;
			values = reinterpret_cast<int32_t*>(itr.get(sz));
		}
		else
		{
			values = nullptr;
		}
	}


	//--------------------------------------------------------------------------------------------------------------------------------

	void FloatArrayProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<uint32_t>(pid, count);
		if(count > 0)
		{
			SERIALIZER_ASSERT(values != nullptr);
			auto sz = sizeof(float) * count;
			std::memcpy(itr.get(sz), values, sz);
		}
	}

	void FloatArrayProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<uint32_t>(pid);
		if(v == nullptr)
			return;

		count = *v;
		if(count > 0)
		{
			auto sz = sizeof(float) * count;
			values = reinterpret_cast<float*>(itr.get(sz));
		}
		else
		{
			values = nullptr;
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	void UnsignedArrayProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<uint32_t>(pid, count);
		if(count > 0)
		{
			SERIALIZER_ASSERT(values != nullptr);
			auto sz = sizeof(uint32_t) * count;
			std::memcpy(itr.get(sz), values, sz);
		}
	}

	void UnsignedArrayProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<uint32_t>(pid);
		if(v == nullptr)
			return;

		count = *v;
		if(count > 0)
		{
			auto sz = sizeof(uint32_t) * count;
			values = reinterpret_cast<uint32_t*>(itr.get(sz));
		}
		else
		{
			values = nullptr;
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	void StringProperty::write(const uint32_t pid, data_iterator& itr) const
	{
		itr.push_value<uint32_t>(pid, length);
		if(length > 0)
		{
			auto zlen = length + 1;
			auto tlen = _alignTo4(zlen);
			SERIALIZER_ASSERT(cstr != nullptr);
			auto* ro = itr.get(tlen);
			std::memcpy(ro, cstr, zlen);
			for (std::size_t i = length; i < tlen; i++)
				ro[i] = '\0';
		}
	}
	void StringProperty::parse(const uint32_t pid, data_iterator& itr)
	{
		auto* v = itr.pop_value<uint32_t>(pid);
		if(v == nullptr)
			return;

		length = *v;
		if(length > 0)
		{
			auto sz = _alignTo4(length + 1);
			cstr = reinterpret_cast<const char*>(itr.get(sz));
		}
		else
		{
			cstr = "";
		}
	}

	std::size_t StringProperty::binsize() const
	{
		auto r = _detail::priperty_header_size() + _detail::unsigned_size();
		if(length > 0)
			r += _alignTo4(length + 1);
		return r;
	}
}
