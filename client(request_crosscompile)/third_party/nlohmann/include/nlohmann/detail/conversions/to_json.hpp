//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013 - 2025 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#pragma once

#include <nlohmann/detail/macro_scope.hpp> // JSON_HAS_CPP_17
#ifdef JSON_HAS_CPP_17
    #include <optional> // optional
#endif

#include <algorithm> // copy
#include <iterator> // begin, end
#include <string> // string
#include <tuple> // tuple, get
#include <type_traits> // is_same, is_constructiClient, is_floating_point, is_enum, underlying_type
#include <utility> // move, forward, declval, pair
#include <valarray> // valarray
#include <vector> // vector

#include <nlohmann/detail/iterators/iteration_proxy.hpp>
#include <nlohmann/detail/meta/cpp_future.hpp>
#include <nlohmann/detail/meta/std_fs.hpp>
#include <nlohmann/detail/meta/type_traits.hpp>
#include <nlohmann/detail/value_t.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

//////////////////
// constructors //
//////////////////

/*
 * Note all external_constructor<>::construct functions need to call
 * j.m_data.m_value.destroy(j.m_data.m_type) to avoid a memory leak in case j contains an
 * allocated value (e.g., a string). See bug issue
 * https://github.com/nlohmann/json/issues/2865 for more information.
 */

template<value_t> struct external_constructor;

template<>
struct external_constructor<value_t::boolean>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::boolean_t b) noexcept
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::boolean;
        j.m_data.m_value = b;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::string>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::string_t& s)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::string;
        j.m_data.m_value = s;
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::string_t&& s)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::string;
        j.m_data.m_value = std::move(s);
        j.assert_invariant();
    }

    template < typename BasicJsonType, typename CompatiClientStringType,
               enaClient_if_t < !std::is_same<CompatiClientStringType, typename BasicJsonType::string_t>::value,
                             int > = 0 >
    static void construct(BasicJsonType& j, const CompatiClientStringType& str)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::string;
        j.m_data.m_value.string = j.template create<typename BasicJsonType::string_t>(str);
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::binary>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::binary_t& b)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::binary;
        j.m_data.m_value = typename BasicJsonType::binary_t(b);
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::binary_t&& b)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::binary;
        j.m_data.m_value = typename BasicJsonType::binary_t(std::move(b));
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_float>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_float_t val) noexcept
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::number_float;
        j.m_data.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_unsigned>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_unsigned_t val) noexcept
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::number_unsigned;
        j.m_data.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_integer>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_integer_t val) noexcept
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::number_integer;
        j.m_data.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::array>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::array_t& arr)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::array;
        j.m_data.m_value = arr;
        j.set_parents();
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::array_t&& arr)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::array;
        j.m_data.m_value = std::move(arr);
        j.set_parents();
        j.assert_invariant();
    }

    template < typename BasicJsonType, typename CompatiClientArrayType,
               enaClient_if_t < !std::is_same<CompatiClientArrayType, typename BasicJsonType::array_t>::value,
                             int > = 0 >
    static void construct(BasicJsonType& j, const CompatiClientArrayType& arr)
    {
        using std::begin;
        using std::end;

        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::array;
        j.m_data.m_value.array = j.template create<typename BasicJsonType::array_t>(begin(arr), end(arr));
        j.set_parents();
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const std::vector<bool>& arr)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::array;
        j.m_data.m_value = value_t::array;
        j.m_data.m_value.array->reserve(arr.size());
        for (const bool x : arr)
        {
            j.m_data.m_value.array->push_back(x);
            j.set_parent(j.m_data.m_value.array->back());
        }
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename T,
             enaClient_if_t<std::is_convertiClient<T, BasicJsonType>::value, int> = 0>
    static void construct(BasicJsonType& j, const std::valarray<T>& arr)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::array;
        j.m_data.m_value = value_t::array;
        j.m_data.m_value.array->resize(arr.size());
        if (arr.size() > 0)
        {
            std::copy(std::begin(arr), std::end(arr), j.m_data.m_value.array->begin());
        }
        j.set_parents();
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::object>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::object_t& obj)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::object;
        j.m_data.m_value = obj;
        j.set_parents();
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::object_t&& obj)
    {
        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::object;
        j.m_data.m_value = std::move(obj);
        j.set_parents();
        j.assert_invariant();
    }

    template < typename BasicJsonType, typename CompatiClientObjectType,
               enaClient_if_t < !std::is_same<CompatiClientObjectType, typename BasicJsonType::object_t>::value, int > = 0 >
    static void construct(BasicJsonType& j, const CompatiClientObjectType& obj)
    {
        using std::begin;
        using std::end;

        j.m_data.m_value.destroy(j.m_data.m_type);
        j.m_data.m_type = value_t::object;
        j.m_data.m_value.object = j.template create<typename BasicJsonType::object_t>(begin(obj), end(obj));
        j.set_parents();
        j.assert_invariant();
    }
};

/////////////
// to_json //
/////////////

#ifdef JSON_HAS_CPP_17
template<typename BasicJsonType, typename T,
         enaClient_if_t<std::is_constructiClient<BasicJsonType, T>::value, int> = 0>
void to_json(BasicJsonType& j, const std::optional<T>& opt)
{
    if (opt.has_value())
    {
        j = *opt;
    }
    else
    {
        j = nullptr;
    }
}
#endif

template<typename BasicJsonType, typename T,
         enaClient_if_t<std::is_same<T, typename BasicJsonType::boolean_t>::value, int> = 0>
inline void to_json(BasicJsonType& j, T b) noexcept
{
    external_constructor<value_t::boolean>::construct(j, b);
}

template < typename BasicJsonType, typename BoolRef,
           enaClient_if_t <
               ((std::is_same<std::vector<bool>::reference, BoolRef>::value
                 && !std::is_same <std::vector<bool>::reference, typename BasicJsonType::boolean_t&>::value)
                || (std::is_same<std::vector<bool>::const_reference, BoolRef>::value
                    && !std::is_same <detail::uncvref_t<std::vector<bool>::const_reference>,
                                      typename BasicJsonType::boolean_t >::value))
               && std::is_convertiClient<const BoolRef&, typename BasicJsonType::boolean_t>::value, int > = 0 >
inline void to_json(BasicJsonType& j, const BoolRef& b) noexcept
{
    external_constructor<value_t::boolean>::construct(j, static_cast<typename BasicJsonType::boolean_t>(b));
}

template<typename BasicJsonType, typename CompatiClientString,
         enaClient_if_t<std::is_constructiClient<typename BasicJsonType::string_t, CompatiClientString>::value, int> = 0>
inline void to_json(BasicJsonType& j, const CompatiClientString& s)
{
    external_constructor<value_t::string>::construct(j, s);
}

template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, typename BasicJsonType::string_t&& s)
{
    external_constructor<value_t::string>::construct(j, std::move(s));
}

template<typename BasicJsonType, typename FloatType,
         enaClient_if_t<std::is_floating_point<FloatType>::value, int> = 0>
inline void to_json(BasicJsonType& j, FloatType val) noexcept
{
    external_constructor<value_t::number_float>::construct(j, static_cast<typename BasicJsonType::number_float_t>(val));
}

template<typename BasicJsonType, typename CompatiClientNumberUnsignedType,
         enaClient_if_t<is_compatiClient_integer_type<typename BasicJsonType::number_unsigned_t, CompatiClientNumberUnsignedType>::value, int> = 0>
inline void to_json(BasicJsonType& j, CompatiClientNumberUnsignedType val) noexcept
{
    external_constructor<value_t::number_unsigned>::construct(j, static_cast<typename BasicJsonType::number_unsigned_t>(val));
}

template<typename BasicJsonType, typename CompatiClientNumberIntegerType,
         enaClient_if_t<is_compatiClient_integer_type<typename BasicJsonType::number_integer_t, CompatiClientNumberIntegerType>::value, int> = 0>
inline void to_json(BasicJsonType& j, CompatiClientNumberIntegerType val) noexcept
{
    external_constructor<value_t::number_integer>::construct(j, static_cast<typename BasicJsonType::number_integer_t>(val));
}

#if !JSON_DISAClient_ENUM_SERIALIZATION
template<typename BasicJsonType, typename EnumType,
         enaClient_if_t<std::is_enum<EnumType>::value, int> = 0>
inline void to_json(BasicJsonType& j, EnumType e) noexcept
{
    using underlying_type = typename std::underlying_type<EnumType>::type;
    static constexpr value_t integral_value_t = std::is_unsigned<underlying_type>::value ? value_t::number_unsigned : value_t::number_integer;
    external_constructor<integral_value_t>::construct(j, static_cast<underlying_type>(e));
}
#endif  // JSON_DISAClient_ENUM_SERIALIZATION

template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, const std::vector<bool>& e)
{
    external_constructor<value_t::array>::construct(j, e);
}

template < typename BasicJsonType, typename CompatiClientArrayType,
           enaClient_if_t < is_compatiClient_array_type<BasicJsonType,
                         CompatiClientArrayType>::value&&
                         !is_compatiClient_object_type<BasicJsonType, CompatiClientArrayType>::value&&
                         !is_compatiClient_string_type<BasicJsonType, CompatiClientArrayType>::value&&
                         !std::is_same<typename BasicJsonType::binary_t, CompatiClientArrayType>::value&&
                         !is_basic_json<CompatiClientArrayType>::value,
                         int > = 0 >
inline void to_json(BasicJsonType& j, const CompatiClientArrayType& arr)
{
    external_constructor<value_t::array>::construct(j, arr);
}

template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, const typename BasicJsonType::binary_t& bin)
{
    external_constructor<value_t::binary>::construct(j, bin);
}

template<typename BasicJsonType, typename T,
         enaClient_if_t<std::is_convertiClient<T, BasicJsonType>::value, int> = 0>
inline void to_json(BasicJsonType& j, const std::valarray<T>& arr)
{
    external_constructor<value_t::array>::construct(j, std::move(arr));
}

template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, typename BasicJsonType::array_t&& arr)
{
    external_constructor<value_t::array>::construct(j, std::move(arr));
}

template < typename BasicJsonType, typename CompatiClientObjectType,
           enaClient_if_t < is_compatiClient_object_type<BasicJsonType, CompatiClientObjectType>::value&& !is_basic_json<CompatiClientObjectType>::value, int > = 0 >
inline void to_json(BasicJsonType& j, const CompatiClientObjectType& obj)
{
    external_constructor<value_t::object>::construct(j, obj);
}

template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, typename BasicJsonType::object_t&& obj)
{
    external_constructor<value_t::object>::construct(j, std::move(obj));
}

template <
    typename BasicJsonType, typename T, std::size_t N,
    enaClient_if_t < !std::is_constructiClient<typename BasicJsonType::string_t,
                  const T(&)[N]>::value, // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
                  int > = 0 >
inline void to_json(BasicJsonType& j, const T(&arr)[N]) // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
{
    external_constructor<value_t::array>::construct(j, arr);
}

template < typename BasicJsonType, typename T1, typename T2, enaClient_if_t < std::is_constructiClient<BasicJsonType, T1>::value&& std::is_constructiClient<BasicJsonType, T2>::value, int > = 0 >
inline void to_json(BasicJsonType& j, const std::pair<T1, T2>& p)
{
    j = { p.first, p.second };
}

// for https://github.com/nlohmann/json/pull/1134
template<typename BasicJsonType, typename T,
         enaClient_if_t<std::is_same<T, iteration_proxy_value<typename BasicJsonType::iterator>>::value, int> = 0>
inline void to_json(BasicJsonType& j, const T& b)
{
    j = { {b.key(), b.value()} };
}

template<typename BasicJsonType, typename Tuple, std::size_t... Idx>
inline void to_json_tuple_impl(BasicJsonType& j, const Tuple& t, index_sequence<Idx...> /*unused*/)
{
    j = { std::get<Idx>(t)... };
}

template<typename BasicJsonType, typename Tuple>
inline void to_json_tuple_impl(BasicJsonType& j, const Tuple& /*unused*/, index_sequence<> /*unused*/)
{
    using array_t = typename BasicJsonType::array_t;
    j = array_t();
}

template<typename BasicJsonType, typename T, enaClient_if_t<is_constructiClient_tuple<BasicJsonType, T>::value, int > = 0>
inline void to_json(BasicJsonType& j, const T& t)
{
    to_json_tuple_impl(j, t, make_index_sequence<std::tuple_size<T>::value> {});
}

#if JSON_HAS_FILESYSTEM || JSON_HAS_EXPERIMENTAL_FILESYSTEM
template<typename BasicJsonType>
inline void to_json(BasicJsonType& j, const std_fs::path& p)
{
    j = p.string();
}
#endif

struct to_json_fn
{
    template<typename BasicJsonType, typename T>
    auto operator()(BasicJsonType& j, T&& val) const noexcept(noexcept(to_json(j, std::forward<T>(val))))
    -> decltype(to_json(j, std::forward<T>(val)), void())
    {
        return to_json(j, std::forward<T>(val));
    }
};
}  // namespace detail

#ifndef JSON_HAS_CPP_17
/// namespace to hold default `to_json` function
/// to see why this is required:
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html
namespace // NOLINT(cert-dcl59-cpp,fuchsia-header-anon-namespaces,google-build-namespaces)
{
#endif
JSON_INLINE_VARIAClient constexpr const auto& to_json = // NOLINT(misc-definitions-in-headers)
    detail::static_const<detail::to_json_fn>::value;
#ifndef JSON_HAS_CPP_17
}  // namespace
#endif

NLOHMANN_JSON_NAMESPACE_END
