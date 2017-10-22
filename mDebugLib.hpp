#pragma onece
#ifndef DEBUG_LIB_HPP__
#define DEBUG_LIB_HPP__ "multi@mDebuglib.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of debug output macro-set.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
*	PAIRED WITH: Debuglib.cpp
**/

//Create one debug.hpp file in your project with macro presets for this lib
#include "debug.hpp"

#ifdef DEBUG_LIB_FILE_LOG
	#ifndef DEBUG_OUT
		#include <fstream>
		//Output variable name macro def : to avoid name conflict
		#ifndef DEBUG_LIB_LOG_FILE_VAR_NAME
			#define DEBUG_LIB_LOG_FILE_VAR_NAME __Debug_Lib_Log
		#endif
		//Std log file path + name
		#ifndef DEBUG_LIB_LOG_FILE_NAME
			#define DEBUG_LIB_LOG_FILE_NAME "log.dat"
		#endif
		namespace DebugLib {
			extern std::ofstream DEBUG_LIB_LOG_FILE_VAR_NAME;
		}
		#define DEBUG_OUT DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
	#endif
#else
	#include <iostream>
	//Output stream
	#ifndef DEBUG_OUT
		#define DEBUG_OUT std::clog
	#endif
#endif

#ifdef DEBUG_LIB_LOG_THREAD_SAFETY
	//Async access control
	//Output mutex name macro def : to avoid name conflict
	#ifndef DEBUG_LIB_LOG_MUTEX_VAR_NAME
		#define DEBUG_LIB_LOG_MUTEX_VAR_NAME __Debug_Lib_Log_Mutex
	#endif

	#include <mutex>
	namespace DebugLib {
		extern std::mutex DEBUG_LIB_LOG_MUTEX_VAR_NAME;
	}
#endif

//New line
#ifndef DEBUG_LIB_NEXT_LINE
	#define DEBUG_LIB_NEXT_LINE "\n"
#endif

#ifdef DEBUG
	//Debug write macro
	//Allow to output one value to debug stream with new line afterwards.
	#define DWRITE1(x) DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE
	//Allow to output one value to debug stream with new line afterwards.
	#define DEBUG_WRITE1(x) DWRITE1(x)
	//Allow to output two values to debug stream with new line afterwards.
	#define DWRITE2(x,y) DEBUG_OUT << (x) << (y) << DEBUG_LIB_NEXT_LINE
	//Allow to output two values to debug stream with new line afterwards.
	#define DEBUG_WRITE2(x,y) DWRITE2(x,y)
	//Allow to output three values to debug stream with new line afterwards.
	#define DWRITE3(x,y,z) DEBUG_OUT << (x) << (y) << (z) << DEBUG_LIB_NEXT_LINE
	//Allow to output three values to debug stream with new line afterwards.
	#define DEBUG_WRITE3(x,y,z) DWRITE3(x,y,z)
	//Allow to output four values to debug stream with new line afterwards.
	#define DWRITE4(x,y,z,w) DEBUG_OUT << (x) << (y) << (z) << (w) << DEBUG_LIB_NEXT_LINE
	//Allow to output four values to debug stream with new line afterwards.
	#define DEBUG_WRITE4(x,y,z,w) DWRITE4(x,y,z,w)
#else
	#define DWRITE1(x) {}
	#define DEBUG_WRITE1(x) {}
	#define DWRITE2(x,y) {}
	#define DEBUG_WRITE2(x,y) {}
	#define DWRITE3(x,y,z) {}
	#define DEBUG_WRITE3(x,y,z) {}
	#define DWRITE4(x,y,z,w) {}
	#define DEBUG_WRITE4(x,y,z,w) {}
#endif

//First line of new massege
#ifndef DEBUG_NEW_MESSAGE
	#ifdef DEBUG
		#ifdef DEBUG_LIB_LOG_THREAD_SAFETY
			//Output lock guard for global mutex var name macro def : to avoid name conflict
			#ifndef DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME
				#define DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME __Debug_Lib_Msg_Scope_Lg;
			#endif
			#define DEBUG_NEW_MESSAGE(x) { \
				std::lock_guard<std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(DebugLib::DEBUG_LIB_LOG_MUTEX_VAR_NAME);\
				DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE;
		#else
			#define DEBUG_NEW_MESSAGE(x) { \
				DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE;
		#endif	
	#else
		#define DEBUG_NEW_MESSAGE(x) { while(false) {
	#endif
#endif

//End line and flush
#ifndef DEBUG_END_MESSAGE
	#ifdef DEBUG
		#define DEBUG_END_MESSAGE DEBUG_OUT << std::flush; }
	#else
		#define DEBUG_END_MESSAGE }}
	#endif
#endif

//End message, flush and exit program with 'exitcode'
#ifndef DEBUG_END_MESSAGE_AND_EXIT
	#include <cstdlib>
	#ifdef DEBUG 
		#define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
				DEBUG_OUT << std::flush; \
				std::exit((exitcode)); }
	#else
		#define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
			} std::exit((exitcode)); }
	#endif
#endif

#endif