#pragma once

#ifdef EDITOR_BUILD
	#ifdef ENGINE_DLL_EXPORT
		#define MOONLIT_API __declspec(dllexport)
	#else
		#define MOONLIT_API __declspec(dllimport)
	#endif // ENGINE_DLL_EXPORT
#else
	#define MOONLIT_API
#endif // DLL_EXPORT
