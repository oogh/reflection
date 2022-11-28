#ifndef REFLECTION_H
#define REFLECTION_H

#include <tuple>
#include <type_traits>
#include "nlohmann/json.hpp"

template <typename T>
inline constexpr auto Schema() {
    return std::make_tuple();
}

// define class
#define DEFINE_SCHEMA(Class, ...) \
    template <> \
    inline constexpr auto Schema<Class>() { \
        using _Class = Class; \
        return std::make_tuple(__VA_ARGS__); \
    }

// define filed
#define DEFINE_SCHEMA_FILED(Filed) \
    std::make_tuple(&_Class::Filed, #Filed)


// unfreeze non-public filed
#define UNFREEZE_NON_PUBLIC_FILED(Class) \
    friend inline constexpr auto Schema<Class>();


/////////////////////////////////////////
/// foreach tuple use for nlohmann::json
/////////////////////////////////////////

namespace detail {
    template <typename T, typename Fn, std::size_t... S>
    inline constexpr void FOREACH_TUPLE(T&& tuple, Fn&& func, std::index_sequence<S...>) {
        std::initializer_list<int>{(func(std::get<S>(std::forward<T>(tuple))), 0)...};
    }

    template <typename T, typename Fn>
    inline constexpr void FOREACH_TUPLE(T&& tuple, Fn&& func) {
        FOREACH_TUPLE(std::forward<T>(tuple),
                      std::forward<Fn>(func),
                      std::make_index_sequence<std::tuple_size<std::decay_t<T>>::value>{});
    }

    template <typename T>
    struct is_field_pointer : std::false_type {};

    template <typename C, typename T>
    struct is_field_pointer<T C::*> : std::true_type {};

    template <typename T>
    constexpr auto is_field_pointer_v = is_field_pointer<T>::value; 
} // namespace detail (from nlohmann::json)

template <typename T, typename Fn>
inline constexpr void FOREACH_FILED(T&& tuple, Fn&& func) {
    const auto schema = Schema<std::decay_t<T>>();
    static_assert(std::tuple_size<decltype(schema)>::value != 0, 
                  "Schema<T>() for type T should be specialized to return Filed tuples, like(&T::filed, filed), ....");
    detail::FOREACH_TUPLE(schema, [&tuple, &func](auto&& filed) {
        using Filed = std::decay_t<decltype(filed)>;
        static_assert(std::tuple_size<Filed>::value >= 2 && detail::is_field_pointer_v<std::tuple_element_t<0, Filed>>, 
                  "Field tuple should be (&T::filed, filed)");
        func(tuple.*(std::get<0>(std::forward<decltype(filed)>(filed))), 
             std::get<1>(std::forward<decltype(filed)>(filed)));
    });
}

template <typename T>
constexpr bool schema_defined = std::tuple_size<decltype(Schema<T>())>::value;

// see https://json.nlohmann.me/api/adl_serializer/
namespace nlohmann {
    template <typename T>
    struct adl_serializer<T, std::enable_if_t<schema_defined<T>>> {
        template<typename BasicJsonType>
        static void to_json(BasicJsonType& j, const T& value) {
            FOREACH_FILED(value, [&j](auto&& filed, auto&& name) {
                j[name] = filed;
            });
        }

        template<typename BasicJsonType>
        static void from_json(const BasicJsonType& j, T& value) {
            FOREACH_FILED(value, [&j](auto&& filed, auto&& name) {
                j.at(name).get_to(filed);
            });
        }
    };
} // namespace nlohmann

#endif // REFLECTION