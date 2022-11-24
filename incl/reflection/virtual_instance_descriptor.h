
#pragma once

#include "reflection_config.h"
#ifdef __cpp_lib_source_location
	#include <source_location>
#include <string_view>
#endif

namespace sg
{

	struct compiletime_class_identifier
	{
	protected:
		const char* 		name = nullptr;
		const std::size_t 	size = 0;//name size

	public:
		inline bool operator == (const compiletime_class_identifier& other) const
		{
			return name == other.name && size == other.size;
		}
		inline bool operator != (const compiletime_class_identifier& other) const
		{
			return name != other.name || size != other.size;
		}
		inline bool operator < (const compiletime_class_identifier& other) const
		{
			if(name == other.name)
				return size < other.size;
			return name < other.name;
		}

	public:
		constexpr compiletime_class_identifier(const char* s, const std::size_t sz)
			:name(s)
			,size(sz)
		{}

		static constexpr compiletime_class_identifier create(const char * n)
		{
			const char* s = n;
			std::size_t sz = 0;
			while (*s++ != '\0') sz++;
			return compiletime_class_identifier{n,sz};
		}

#ifdef __cpp_lib_source_location
		static constexpr compiletime_class_identifier create(const std::source_location& location)
		{
			return create(location.function_name());
		}
#endif

	};

	//--------------------------------------------------------------------------------------------------------------------------------

	class InstanceDescriptor
	{
	protected:
		template <class>
		friend struct impl_instance_descriptor_helper;

		virtual compiletime_class_identifier _get_final_instance_id() const = 0;
		virtual bool _is_instance_recursive(const compiletime_class_identifier& ik) const = 0;

	public:
		template <class T>
		inline T* cast();
		template <class T>
		inline const T* cast() const;

		template <class T>
		inline bool isinstance() const;

		inline bool isinstance(const compiletime_class_identifier& tp) const;
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	template <class BASE>
	struct impl_instance_descriptor_helper
	{
		template <class T>
		inline static bool impl_is_instance(const compiletime_class_identifier& ik, const T* th)
		{
			if(T::_get_type_id() == ik)
				return true;
			return static_cast<const BASE*>(th)->BASE::_is_instance_recursive(ik);
		}
	};

	template <>
	struct impl_instance_descriptor_helper <void>
	{
		template <class T>
		inline static bool impl_is_instance(const compiletime_class_identifier& ik, const T*)
		{
			return T::_get_type_id() == ik;
		}

		template <class T>
		inline static compiletime_class_identifier impl_get_type_id()
		{
			return T::_get_type_id();
		}
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	template <class T>
	struct class_identifier_from
	{
		inline static compiletime_class_identifier value()
		{
			return impl_instance_descriptor_helper<void>::impl_get_type_id<T>();
		}
	};
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class T>
	inline T* InstanceDescriptor::cast()
	{
		REFLECTION_ASSERT(_is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_type_id<T>()));
		return static_cast<T*>(this);
	}
	template <class T>
	inline const T* InstanceDescriptor::cast() const
	{
		REFLECTION_ASSERT(_is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_type_id<T>()));
		return static_cast<const T*>(this);
	}
	template <class T>
	inline bool InstanceDescriptor::isinstance() const
	{
		return _is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_type_id<T>());
	}
	inline bool InstanceDescriptor::isinstance(const compiletime_class_identifier& tp) const
	{
		return _is_instance_recursive(tp);
	}

}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------


#define IMPL_TYPEID_CUSTOM_STR(STR) \
	static sg::compiletime_class_identifier _get_type_id() \
	{ \
		constexpr auto r = sg::compiletime_class_identifier::create(#STR); \
		return r; \
	}

//--------------------------------------------------------------------------------------------------------------------------------

#ifdef __cpp_lib_source_location

#define IMPL_TYPEID_DEFAULT_STR() \
	static sg::compiletime_class_identifier _get_type_id() \
	{ \
		constexpr auto r = sg::compiletime_class_identifier::create(std::source_location::current()); \
		return r; \
	}


#else

// https://stackoverflow.com/questions/19343205/c-concatenating-file-and-line-macros/19343239
#define SRCGEN_TO_STRING_MACRO_INTERNAL(A) #A
#define SRCGEN_LINET_TO_STR_MACRO(A) SRCGEN_TO_STRING_MACRO_INTERNAL(A)
#define SRCGEN_LOCATION __FILE__ "::" SRCGEN_LINET_TO_STR_MACRO(__LINE__)

#define IMPL_TYPEID_DEFAULT_STR() \
	static sg::compiletime_class_identifier _get_type_id() \
	{ \
		constexpr auto r = sg::compiletime_class_identifier::create(SRCGEN_LOCATION); \
		return r; \
	}

#endif

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

#define IMPL_CLASS_VIRTUAL_FUNCTIONS(BASE) \
	template <class> friend struct sg::impl_instance_descriptor_helper; \
	virtual sg::compiletime_class_identifier _get_final_instance_id() const  override \
	{ \
		return _get_type_id(); \
	} \
	virtual bool _is_instance_recursive(const sg::compiletime_class_identifier& ik) const override \
	{ \
		return sg::impl_instance_descriptor_helper<BASE>::impl_is_instance(ik, this); \
	}

//--------------------------------------------------------------------------------------------------------------------------------

#define CUSTOM_CLASS_NAME(STR) \
	IMPL_TYPEID_CUSTOM_STR(STR); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(void)


#define DEFAULT_CLASS_NAME() \
	IMPL_TYPEID_DEFAULT_STR(); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(void)

#define CUSTOM_CLASS_NAME_EX(STR,B) \
	IMPL_TYPEID_CUSTOM_STR(STR); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(B)


#define DEFAULT_CLASS_NAME_EX(B) \
	IMPL_TYPEID_DEFAULT_STR(); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(B)





