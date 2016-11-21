#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#ifdef TARGET_OS_IPHONE
@class CAEAGLLayer;
#endif
#endif

namespace pgn {

#ifdef _WIN32
typedef HDC Display;
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#ifdef TARGET_OS_IPHONE
typedef CAEAGLLayer* Display;
#endif
#endif

#ifdef __psp2__
typedef void* Display;
#endif

}
