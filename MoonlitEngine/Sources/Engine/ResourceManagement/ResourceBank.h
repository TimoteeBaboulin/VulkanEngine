#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Moonlit::ResourceManagement
{
	constexpr int ResourceBankDefaultSize = 10;
	//
	////Forward declaration
	//template <class T>
	//class ResourceBank;

	template <class T>
	struct ResourcePair
	{
		std::string Name;
		T Resource;
		std::shared_ptr<T> ResourcePtr;
	};

	template <typename RESOURCE_TYPE>
	class ResourceHandle;

	/// <summary>
	/// Generic class holding the loaded resources and implementing load operations 
	/// </summary>
	template <class RESOURCE_TYPE>
	class ResourceBank
	{
	public:
		using PAIR_TYPE = ResourcePair<RESOURCE_TYPE>;
		using HANDLE_TYPE = ResourceHandle<RESOURCE_TYPE>;

		static inline ResourceBank<RESOURCE_TYPE>* Instance;

		ResourceBank();
		virtual ~ResourceBank() = default;

		// Getters
		std::vector<HANDLE_TYPE> GetAllResources();
		HANDLE_TYPE Get(std::string _name);
		bool Exist(std::string _name);
		PAIR_TYPE& operator[](size_t index);

		// Load/Unload
		virtual bool TryLoad(std::string _filepath) = 0;
		void TryUnloadUnusedResources();

	protected:
		std::vector<ResourcePair<RESOURCE_TYPE>> m_resources;
	};

}