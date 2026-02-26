#pragma once

#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "../../MoonlitExport.h"
#include "ResourceBank.h"



namespace Moonlit::ResourceManagement
{
	/// <summary>
	/// Static class handling ResourceBank interface for easy loading of different resources
	/// </summary>
	class MOONLIT_API ResourceManager
	{
		// Static
	public:
		template <class T>
		static void RegisterResourceBank(ResourceBank<T>* _bank)
		{
			Instance()->m_resourceBanks[std::type_index(typeid(T))] = _bank;
		}
		template <class T>
		static bool TryGetResource(std::string _name, std::shared_ptr<T>& _outResource)
		{
			return Instance()->TryGetResourceInstance<T>(_name, _outResource);
		}
		template <class T>
		static bool TryLoadResource(std::string _path)
		{
			return Instance()->TryLoadResourceInstance<T>(_path);
		}

		template <class T>
		std::vector<ResourcePair<T>> GetAllResourcesOfType()
		{
			std::vector<ResourcePair<T>> resources;
			auto it = m_resourceBanks.find(std::type_index(typeid(T)));
			if (it != m_resourceBanks.end())
			{
				ResourceBank<T>* bank = static_cast<ResourceBank<T>*>(it->second);
				return bank->GetAllResources();
			}
			return resources;
		}

		static ResourceManager* Instance();

		// Instance
	public:
		ResourceManager();

	private:
		template <class T>
		bool TryGetResourceInstance(std::string _name, std::shared_ptr<T>& _outResource)
		{
			auto it = m_resourceBanks.find(std::type_index(typeid(T)));
			if (it != m_resourceBanks.end())
			{
				ResourceBank<T>* bank = static_cast<ResourceBank<T>*>(it->second);
				_outResource = bank->Get(_name);
				return _outResource != nullptr;
			}

			return false;
		}

		template <class T>
		bool TryLoadResourceInstance(std::string _path)
		{
			auto it = m_resourceBanks.find(std::type_index(typeid(T)));
			if (it != m_resourceBanks.end())
			{
				ResourceBank<T>* bank = static_cast<ResourceBank<T>*>(it->second);
				return bank->TryLoad(_path);
			}

			return false;
		}

		void LoadFileResource(std::string _filepath);

		//Link the resource type to its bank
		std::map<::std::type_index, void*> m_resourceBanks;
	};
}