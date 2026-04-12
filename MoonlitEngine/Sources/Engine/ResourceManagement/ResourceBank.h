#pragma once

#include <string>
#include <vector>
#include <map>
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

		static inline ResourceBank* Instance;

		ResourceBank();
		virtual ~ResourceBank() = default;

		// Getters
		std::vector<HANDLE_TYPE> GetAllResources();
		std::vector<std::string> GetAllNames();

		HANDLE_TYPE Get(std::string _name);
		bool Exist(std::string _name);
		RESOURCE_TYPE& operator[](std::string _name);

		// Load/Unload
		virtual bool TryLoad(std::string _filepath) = 0;
		void TryUnloadUnusedResources();

	protected:
		std::map<std::string, RESOURCE_TYPE> m_resources;

		void InsertResource(const std::string& _name, const RESOURCE_TYPE& _resource);
	};
}

#include "ResourceBank.ipp"
