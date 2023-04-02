
#pragma once

#include <devtiny.h>

#ifdef SERIALIZER_TESTING
	#include <ttf.h>
	#define SERIALIZER_ASSERT TTF_ASSERT
#else
	#define SERIALIZER_ASSERT DEV_ASSERT
#endif

namespace ss
{
	using byte_t = unsigned char;
}

#include <cstring>
#include <string_view>
