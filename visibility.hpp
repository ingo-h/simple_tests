#ifndef UPNPLIB_INCLUDE_VISIBILITY_HPP
#define UPNPLIB_INCLUDE_VISIBILITY_HPP
// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-09-19

// C++ visibility support
//-----------------------
// Reference: https://gcc.gnu.org/wiki/Visibility
// List all exported symbols with: nm -C -D <library>.so

// Generic helper definitions for shared library support
// clang-format off
#if defined _WIN32 || defined __CYGWIN__
  #define UPNP_HELPER_DLL_IMPORT __declspec(dllimport)
  #define UPNP_HELPER_DLL_EXPORT __declspec(dllexport)
  #define UPNP_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define UPNP_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define UPNP_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define UPNP_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define UPNP_HELPER_DLL_IMPORT
    #define UPNP_HELPER_DLL_EXPORT
    #define UPNP_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define UPNPLIB_API and
// UPNPLIB_LOCAL. UPNPLIB_API is used for the public API symbols. It either DLL imports
// or DLL exports (or does nothing for static build) UPNPLIB_LOCAL is used for
// non-api symbols.

#ifdef UPNPLIB_SHARED // defined if UPNPLIB is compiled as a shared library
  #ifdef UPNPLIB_EXPORTS // defined if we are building the UPNPLIB DLL (instead of using it)
    #define UPNPLIB_API UPNP_HELPER_DLL_EXPORT
  #else
    #define UPNPLIB_API UPNP_HELPER_DLL_IMPORT
  #endif // UPNPLIB_EXPORTS
  #define UPNPLIB_LOCAL UPNP_HELPER_DLL_LOCAL
#else // UPNPLIB_SHARED is not defined: this means UPNPLIB is a static lib.
  #define UPNPLIB_API
  #define UPNPLIB_LOCAL
#endif // UPNPLIB_SHARED

#if (defined _WIN32 || defined __CYGWIN__) && defined UPNPLIB_SHARED
  #define UPNPLIB_EXTERN __declspec(dllimport) extern
#else
  #define UPNPLIB_EXTERN UPNPLIB_API extern
#endif
// clang-format on

#endif // UPNPLIB_INCLUDE_VISIBILITY_HPP
