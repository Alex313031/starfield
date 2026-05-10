#ifndef STARFIELD_FRAMEWORK_H_
#define STARFIELD_FRAMEWORK_H_

// clang-format off
#include "version.h" // Keep this at the top!

#define NOMINMAX

#include <windows.h>  // Main Windows header
#include <commctrl.h> // Common Controls

#include <scrnsave.h> // Screensaver header

// C Runtime Headers
#include <tchar.h> // For TCHAR, and automatically deducing wchar_t type

// C++ STL Headers
#include <algorithm>
#include <random>
#include <string>

#ifndef __cplusplus
 #error APP_NAME requires a C++ compiler
#endif
#if __cplusplus < 201103L
 // For old compilers without constexpr or inline
 #if !defined(constexpr) || !defined(__cpp_constexpr)
  #define constexpr const
 #endif // constexpr
 #if !defined(inline)
  #define inline
 #endif // inline
#endif

// clang-format on

// Alias
#ifndef __FUNC__
 #define __FUNC__ __func__
#endif

// Defines for missing windowsx.h definitions, don't want to inlcude
// the heavy .h file just for this one thing.
#if !defined(GET_X_LPARAM) || !defined(GET_Y_LPARAM)
 #define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
 #define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

// Convert compiler defines to usable bools
inline constexpr bool is_dcheck =
#ifdef DCHECK_ON
    true;
#else
    false;
#endif // DCHECK

inline constexpr bool is_debug =
#if defined(DEBUG) || defined(_DEBUG)
    true;
#else
    false;
#endif // defined(DEBUG) || defined(_DEBUG)

#ifndef CharSizeOf
 #define CharSizeOf(char_t) (sizeof(char_t) / sizeof((char_t)[0]))
#endif // !CharSizeOf

#endif // STARFIELD_FRAMEWORK_H_
