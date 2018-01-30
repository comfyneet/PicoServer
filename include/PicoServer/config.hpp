#ifndef PS_CONFIG_HPP
#define PS_CONFIG_HPP

#ifdef _WIN32
#  define PS_WINDOWS
#else
#  define PS_LINUX
#endif

#ifdef _MSC_VER
#  pragma warning(disable: 4251)
#  define PS_DLLEXPORT __declspec(dllexport)
#  define PS_DLLIMPORT __declspec(dllimport)
#else
#  define PS_DLLEXPORT __attribute__ ((visibility ("default")))
#  define PS_DLLIMPORT
#endif

#ifdef PS_BUILD_DLL
#  define PS_API PS_DLLEXPORT
#elif defined(PS_DLL)
#  define PS_API PS_DLLIMPORT
#else
#  define PS_API
#endif

#endif
