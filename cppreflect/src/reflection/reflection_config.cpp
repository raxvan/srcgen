
#include <reflection/reflection_config.h>

#ifdef REFLECTION_ENABLE_ASSERT
#	include <iostream>
#	include <cassert>
#endif

namespace sg
{

#ifdef REFLECTION_ENABLE_ASSERT
	void reflection_assert_failed(const char* file, const int line, const char* cond)
	{
		std::cerr << "REFLECTION_ASSERT failed in " << file << "(" << line << ")> " << cond << std::endl;
		assert(false);
	}
#endif

}
