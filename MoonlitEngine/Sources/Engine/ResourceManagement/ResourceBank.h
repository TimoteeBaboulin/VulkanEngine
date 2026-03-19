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

	/// <summary>
	/// Generic class holding the loaded resources and implementing load operations 
	/// </summary>
	template <class T>
	class ResourceBank
	{
	public:
		using PAIR_TYPE = ResourcePair<T>;

		static inline ResourceBank<T>* Instance;

		ResourceBank();

		// Getters
		std::vector<PAIR_TYPE> GetAllResources() { return m_resources; }
		std::shared_ptr<T> Get(std::string _name);
		bool Exist(std::string _name);
		PAIR_TYPE& operator[](size_t index);

		// Load/Unload
		virtual bool TryLoad(std::string _filepath) = 0;
		void TryUnloadUnusedResources();

	protected:
		std::vector<ResourcePair<T>> m_resources;
	};

}