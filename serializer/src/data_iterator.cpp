
#include "../incl/data_iterator.h"

namespace ss
{
	bool data_iterator::accept(uint32_t ID)
	{
		auto i = id();
		SERIALIZER_ASSERT(i <= ID);
		if(i == ID)
		{
			start += sizeof(uint32_t);
			return true;
		}
		return false;
	}
	byte_t* data_iterator::get(const std::size_t sz)
	{
		SERIALIZER_ASSERT(start != nullptr && (start + sz) <= end);
		auto* r = start;
		start += sz;
		return r;
	}
}
