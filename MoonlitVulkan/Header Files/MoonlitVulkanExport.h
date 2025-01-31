#ifndef MOONLIT_VULKAN_EXPORT_H
#define MOONLIT_VULKAN_EXPORT_H

# ifdef DYNAMIC_LIBRARY
# define MOONLIT_API __declspec(dllexport)
# else
# ifdef NOLIB
# define MOONLIT_API
# else
# define MOONLIT_API __declspec(dllimport)
# endif
# endif

#endif // !MOONLIT_VULKAN_EXPORT_H
