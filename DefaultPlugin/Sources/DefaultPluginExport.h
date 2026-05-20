#pragma once

#ifdef DEFAULTPLUGIN_EXPORTS
	#define DEFAULTPLUGIN_API __declspec(dllexport)
#else
	#define DEFAULTPLUGIN_API __declspec(dllimport)
#endif
