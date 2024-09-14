#pragma once
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <__filesystem/path.h>
#include <nlohmann/json.hpp>
#include <bytesize.hh>

#define INS_ENABLE_VALIDATION_LAYERS

namespace Insight
{
    using string = std::string;
    using string_view = std::string_view;
    template <typename T>
    using vector = std::vector<T>;
    template <typename T>
    using set = std::set<T>;
    template <typename K, typename V>
    using map = std::map<K, V>;
    template <typename K, typename V>
    using unordered_map = std::unordered_map<K, V>;
    template <typename T>
    using optional = std::optional<T>;
    using path = std::filesystem::path;
    using json = nlohmann::json;

    using bytesize = bytesize::bytesize;

    template <class T, class U>
    concept Derived = std::is_base_of_v<U, T>;

    template <typename T>
    using Unique = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Unique<T> CreateUnique(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using WeakRef = std::weak_ptr<T>;
    template <typename T>
    constexpr WeakRef<T> CreateWeakRef(const Ref<T> &ref)
    {
        return std::weak_ptr<T>(ref);
    }

    using i8 = int8_t;
    using u8 = uint8_t;
    using i16 = int16_t;
    using u16 = uint16_t;
    using i32 = int32_t;
    using u32 = uint32_t;
    using i64 = int64_t;
    using u64 = uint64_t;
    using f32 = float;
    using f64 = double;
}


#include "Log.h"

