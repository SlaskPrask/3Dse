#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifndef WINDOWS
#define WINDOWS
#ifndef PLATFORM
#define PLATFORM
#endif
#ifndef STANDALONE
#define STANDALONE
#endif
#endif
#endif

#if defined(__ANDROID__) || defined(ANDROID)
#ifndef ANDROID
#define ANDROID
#endif
#ifndef PLATFORM
#define PLATFORM
#endif
#ifndef MOBILE
#define MOBILE
#endif
#endif

#if defined(__linux__) || defined(__linux) || defined(_linux) || defined(__LINUX__) || defined(__LINUX) || defined(_LINUX)
#ifndef LINUX
#define LINUX
#ifndef UNIX
#define UNIX
#endif
#ifndef PLATFORM
#define PLATFORM
#endif
#ifndef STANDALONE
#define STANDALONE
#endif
#endif
#endif


#if defined(__APPLE__)
#ifndef MAC
#define MAC
#ifndef UNIX
#define UNIX
#endif
#ifndef PLATFORM
#define PLATFORM
#endif
#ifndef STANDALONE
#define STANDALONE
#endif
#endif
#endif

#ifndef PLATFORM
#error Compiling platform not recognized.
#endif

#ifdef __PRETTY_FUNCTION__
#define _OBJECT_NAME __PRETTY_FUNCTION__
#elif defined __FUNCSIG__
#define _OBJECT_NAME __FUNCSIG__
#else
#define _OBJECT_NAME " Unavailable::"
#endif

#define _DEFSTR1(x) #x
#define _DEFSTR(x) _DEFSTR1(x)

#ifdef DEBUG
#define DEBUGFUNC(ARG) ARG
#else
#define DEBUGFUNC(ARG)
#endif
