
#pragma once

//--------------------------------------------------------------------------------------------------------------------------------

#define REFLECTION_ENABLE_ASSERT

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
#if defined(REFLECTION_TESTING)

#	include <ttf.h>
#	define REFLECTION_ASSERT TTF_ASSERT

#elif defined(REFLECTION_ENABLE_ASSERT)
//--------------------------------------------------------------------------------------------------------------------------------
#	if defined(REFLECTION_WITH_DEV_PLATFORM)

#		include <devtiny.h>
#		define REFLECTION_ASSERT DEV_ASSERT

#	else

//--------------------------------------------------------------------------------------------------------------------------------

namespace cllio
{
	extern "C++" void reflection_assert_failed(const char* file, const int line, const char* cond);
}

#		define REFLECTION_ASSERT(_COND)                                  \
			do                                                       \
			{                                                        \
				if (!(_COND))                                        \
					reflection_assert_failed(__FILE__, __LINE__, #_COND); \
			} while (false)
#		define REFLECTION_ASSERT_FALSE(CSTR_MSG)                       \
			do                                                     \
			{                                                      \
				reflection_assert_failed(__FILE__, __LINE__, CSTR_MSG); \
			} while (false)
#	endif
//--------------------------------------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

#ifndef REFLECTION_ASSERT

#	define REFLECTION_ASSERT(...) \
		do                    \
		{                     \
		} while (false)
#endif

#ifndef REFLECTION_ASSERT_FALSE
#	define REFLECTION_ASSERT_FALSE(...) \
		do                          \
		{                           \
			REFLECTION_ASSERT(false);    \
		} while (false)

#endif
