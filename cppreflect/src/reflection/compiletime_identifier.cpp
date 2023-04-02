#pragma once

#include "../incl/reflection/compiletime_identifier.h"

#include <cstring>
namespace sg
{

	bool compiletime_identifier::is(const std::string_view& sv) const
	{
		REFLECTION_ASSERT(size > 0 && sv.size() > 0);
		if (size != sv.size())
			return false;

		REFLECTION_ASSERT(id != nullptr);

		return std::strncmp(id, sv.data(), size) == 0;
	}

}