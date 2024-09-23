#pragma once

#ifdef DEBUG
#define JSENGINE_PREV_DEBUG
#undef DEBUG
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"

#include <jsapi.h>
#include <js/Exception.h>
#include <js/Modules.h>
#include <js/Conversions.h>
#include <js/Class.h>
#include <js/CompilationAndEvaluation.h>
#include <jsfriendapi.h>
#include <js/PropertySpec.h>
#include <js/PropertyDescriptor.h>
#include <js/PropertyAndElement.h>
#include <js/TracingAPI.h>
#include <js/RootingAPI.h>

#pragma clang diagnostic pop

#ifdef JSENGINE_PREV_DEBUG
#define DEBUG
#undef JSENGINE_PREV_DEBUG
#endif