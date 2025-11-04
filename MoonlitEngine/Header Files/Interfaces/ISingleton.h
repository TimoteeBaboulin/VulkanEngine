#pragma once

/// <summary>
/// This is unused
/// </summary>
template <class T>
class ISingleton
{
public:
static T* Get();

protected:
static T* Instance;
};

template<class T>
inline T* ISingleton<T>::Get()
{
if (Instance == nullptr)
	Instance = new T();
return Instance;
}
