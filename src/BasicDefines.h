#pragma once

/*debug preprocessor ensures writing every application step*/
/*if the application message output is associated with application failure/crash messages are still captured in non _DEBUG mode*/
//#define _DEBUG

#define APPLICATION_NAME "Task Manager"

#if defined(_DEBUG)
#	define APP_BUILD_MODE "Debug version"
#else
#	define APP_BUILD_MODE "Release version"
#endif

#ifdef __linux__
#	define PLATFORM_NAME "Linux"
#elif _WIN32
#	define PLATFORM_NAME "Windows"
#endif