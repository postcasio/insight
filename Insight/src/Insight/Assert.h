#pragma once

#define INS_EXPAND_MACRO(x) x
#define INS_STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)

#ifndef NDEBUG
#define INS_ENABLE_ASSERTS
#include <signal.h>
#define INS_DEBUGBREAK() raise(SIGTRAP)
#else
#define INS_DEBUGBREAK()
#endif

#include "Log.h"
#include <filesystem>
#ifdef INS_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define INS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
{                                                  \
if (!(check))                                  \
{                                              \
INS##type##ERROR(msg, __VA_ARGS__);         \
INS_DEBUGBREAK();                           \
}                                              \
}
#define INS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) INS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define INS_INTERNAL_ASSERT_NO_MSG(type, check) INS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", INS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define INS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define INS_INTERNAL_ASSERT_GET_MACRO(...) INS_EXPAND_MACRO(INS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INS_INTERNAL_ASSERT_WITH_MSG, INS_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define INS_ASSERT(...) INS_EXPAND_MACRO(INS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define INS_ENGINE_ASSERT(...) INS_EXPAND_MACRO(INS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_ENGINE_, __VA_ARGS__))
#else
#define INS_ASSERT(...)
#define INS_ENGINE_ASSERT(...)
#endif