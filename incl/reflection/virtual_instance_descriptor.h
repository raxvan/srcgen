
#pragma once

#include "compiletime_identifier.h"

namespace sg
{

	class InstanceDescriptor
	{
	public:
		struct ITypeVisiter
		{
		public:
			virtual void operator ()(const compiletime_identifier& ci) = 0;
		};
	protected:
		template <class>
		friend struct impl_instance_descriptor_helper;

		virtual compiletime_identifier _get_final_instance_id() const = 0;
		virtual bool				   _is_instance_recursive(const compiletime_identifier& ik) const = 0;
		virtual void 				   _gather_types(ITypeVisiter& visiter) const = 0;
	public:
		template <class T>
		inline T* cast();
		template <class T>
		inline const T* cast() const;

		template <class T>
		inline bool isinstance() const;

		inline bool isinstance(const compiletime_identifier& tp) const;

		inline compiletime_identifier dynamic_type() const;

		inline void visit_types(ITypeVisiter& v) const;
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	template <class BASE>
	struct impl_instance_descriptor_helper
	{
		template <class T>
		inline static bool impl_is_instance(const compiletime_identifier& ik, const T* th)
		{
			if (T::_get_static_type_id() == ik)
				return true;
			return static_cast<const BASE*>(th)->BASE::_is_instance_recursive(ik);
		}
		template <class T>
		inline static void impl_visit_types(InstanceDescriptor::ITypeVisiter& visiter, const T* th)
		{
			visiter(T::_get_static_type_id());
			static_cast<const BASE*>(th)->BASE::_gather_types(visiter);
		}
	};

	template <>
	struct impl_instance_descriptor_helper<void>
	{
		template <class T>
		inline static bool impl_is_instance(const compiletime_identifier& ik, const T*)
		{
			return T::_get_static_type_id() == ik;
		}
		template <class T>
		inline static void impl_visit_types(InstanceDescriptor::ITypeVisiter& visiter, const T*)
		{
			visiter(T::_get_static_type_id());
		}

		template <class T>
		inline static compiletime_identifier impl_get_static_type_id()
		{
			return T::_get_static_type_id();
		}
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------
	template <class T>
	struct class_identifier_from
	{
		inline static compiletime_identifier value()
		{
			return impl_instance_descriptor_helper<void>::impl_get_static_type_id<T>();
		}
	};
	//--------------------------------------------------------------------------------------------------------------------------------

	template <class T>
	inline T* InstanceDescriptor::cast()
	{
		REFLECTION_ASSERT(_is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_static_type_id<T>()));
		return static_cast<T*>(this);
	}
	template <class T>
	inline const T* InstanceDescriptor::cast() const
	{
		REFLECTION_ASSERT(_is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_static_type_id<T>()));
		return static_cast<const T*>(this);
	}
	template <class T>
	inline bool InstanceDescriptor::isinstance() const
	{
		return _is_instance_recursive(impl_instance_descriptor_helper<void>::impl_get_static_type_id<T>());
	}
	inline bool InstanceDescriptor::isinstance(const compiletime_identifier& tp) const
	{
		return _is_instance_recursive(tp);
	}
	inline compiletime_identifier InstanceDescriptor::dynamic_type() const
	{
		return this->_get_final_instance_id();
	}
	inline void InstanceDescriptor::visit_types(ITypeVisiter& v) const
	{
		this->_gather_types(v);
	}

}

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

#define IMPL_TYPEID_CUSTOM_STR(STR)                                  \
	static sg::compiletime_identifier _get_static_type_id()                 \
	{                                                                \
		constexpr auto r = sg::compiletime_identifier::create(#STR); \
		return r;                                                    \
	}

//--------------------------------------------------------------------------------------------------------------------------------

#ifdef __cpp_lib_source_location

#	define IMPL_TYPEID_DEFAULT_STR()                                                               \
		static sg::compiletime_identifier _get_static_type_id()                                            \
		{                                                                                           \
			constexpr auto r = sg::compiletime_identifier::create(std::source_location::current()); \
			return r;                                                                               \
		}

#else

// https://stackoverflow.com/questions/19343205/c-concatenating-file-and-line-macros/19343239
#	define SRCGEN_TO_STRING_MACRO_INTERNAL(A) #	   A
#	define SRCGEN_LINET_TO_STR_MACRO(A) SRCGEN_TO_STRING_MACRO_INTERNAL(A)
#	define SRCGEN_LOCATION __FILE__ "::" SRCGEN_LINET_TO_STR_MACRO(__LINE__)

#	define IMPL_TYPEID_DEFAULT_STR()                                               \
		static sg::compiletime_identifier _get_static_type_id()                            \
		{                                                                           \
			constexpr auto r = sg::compiletime_identifier::create(SRCGEN_LOCATION); \
			return r;                                                               \
		}

#endif

//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

#define IMPL_CLASS_VIRTUAL_FUNCTIONS(BASE)                                                   \
	template <class>                                                                         \
	friend struct sg::impl_instance_descriptor_helper;                                       \
	virtual sg::compiletime_identifier _get_final_instance_id() const override               \
	{                                                                                        \
		return _get_static_type_id();                                                               \
	}                                                                                        \
	virtual bool _is_instance_recursive(const sg::compiletime_identifier& ik) const override \
	{                                                                                        \
		return sg::impl_instance_descriptor_helper<BASE>::impl_is_instance(ik, this);        \
	} \
	virtual void _gather_types(InstanceDescriptor::ITypeVisiter& visiter) const override \
	{ \
		sg::impl_instance_descriptor_helper<BASE>::impl_visit_types(visiter, this); \
	}

//--------------------------------------------------------------------------------------------------------------------------------

#define CUSTOM_CLASS_NAME(STR)   \
	IMPL_TYPEID_CUSTOM_STR(STR); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(void)

#define DEFAULT_CLASS_NAME()   \
	IMPL_TYPEID_DEFAULT_STR(); \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(void)

#define CUSTOM_CLASS_NAME_EX(STR, B) \
	IMPL_TYPEID_CUSTOM_STR(STR);     \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(B)

#define DEFAULT_CLASS_NAME_EX(B) \
	IMPL_TYPEID_DEFAULT_STR();   \
	IMPL_CLASS_VIRTUAL_FUNCTIONS(B)
