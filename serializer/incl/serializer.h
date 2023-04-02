
#pragma once

#include "properties.h"

namespace ss
{

	template <class S, class IS>
	struct struct_serializer_impl;

	template <class S, uint32_t ... IDS>
	struct struct_serializer_impl <S, std::integer_sequence<uint32_t, IDS ...> >
	{
	public:
		template <class FUNC>
		//const void* _func(const std::size_t);
		static std::pair<void*, std::size_t> write(const S& _struct, const FUNC& _func)
		{
			auto header_size = _detail::struct_header_size();
			auto data_size = (... + _gs_detail::member_visiter<S, IDS>::get(_struct).binsize());
			auto total_size = header_size + data_size;

			SERIALIZER_ASSERT(total_size > 0 && total_size < std::numeric_limits<uint32_t>::max());
			byte_t* data_out = (byte_t*) _func(total_size);
			SERIALIZER_ASSERT(data_out != nullptr);

			auto itr = _detail::write_header_struct(S::key(), data_out, (uint32_t)data_size);
			(..., _gs_detail::member_visiter<S, IDS>::get(_struct).write(IDS, itr));
			SERIALIZER_ASSERT((data_out + total_size) == itr.start);
			return {data_out, total_size};
		}

		template <uint32_t I>
		inline static bool _view(S& _struct, data_iterator& itr)
		{
			_gs_detail::member_visiter<S, I>::get(_struct).parse(I, itr);
			return itr.hasdata();
		}

		//const void* _func(const std::size_t);
		static bool parse(S& _struct, std::pair<void*, std::size_t> data)
		{
			data_iterator itr;
			itr.start = reinterpret_cast<byte_t*>(data.first);
			itr.end = itr.start + data.second;
			if(_detail::parse_struct_header(S::key(), itr) == false)
				return false;
			auto r = (... && _view<IDS>(_struct, itr ) );
			SERIALIZER_ASSERT(r == false && itr.hasdata() == false);
			return true;
		}

	};

	template <class S>
	using struct_serializer = struct_serializer_impl<S, std::make_integer_sequence<uint32_t, S::member_count()>>;

	//template <class S, uint32_t ... IDS>
	//using struct_partial_serializer = struct_serializer_impl<S, std::integer_sequence<uint32_t, IDS ...> >;
};
