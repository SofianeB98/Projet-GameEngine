#pragma once

#include <memory>
#include <unordered_map>
#include "EcsBase.h"
#include "SystemBase.h"
#include <assert.h>
#include "ComponentBase.h"
#include "fnv.h"

namespace ECS {

	class SystemManager
	{
		std::unordered_map<uint32_t, UniqueKey> keys{};
		std::unordered_map <uint32_t, std::shared_ptr<SystemBase>> systems{};
		
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem() {
			//const char* typeName = typeid(T).name();
			uint32_t typeName = FNV::fnv1a(typeid(T).name());
			
			assert(systems.find(typeName) == systems.end(), "System " << typeid(T).name() << " already registered");
			
			auto system = std::make_shared<T>();
			systems.insert({ typeName, system });
			
			return system;
		}


		template<typename T>
		void SetUniqueKey(UniqueKey key) {
			uint32_t typeName = FNV::fnv1a(typeid(T).name());
			assert(systems.find(typeName) != systems.end(), "System " << typeid(T).name() << " not registered");
			keys.insert({ typeName, key });
		}

		void EntityDestroyed(Entity entity) {
			for (auto const& pair : systems) {
				auto const& system = pair.second;
				system->entities.erase(entity);
			}
		}

		void EntityKeyChanged(Entity entity, UniqueKey entityKey)
		{
			for(auto const& pair : systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemKey = keys[type];

				// Si les filtres match, on ajoute l'entity à la list du system
				if((entityKey & systemKey) == systemKey)
				{
					system->entities.insert(entity);
				}
				// Sinon on la supprime
				else {
					system->entities.erase(entity);
				}
			}
		}

		void UpdateSystem(float dt) const
		{
			for (auto const& sys : systems)
			{
				sys.second->Update(dt);
			}
		}

		void DeInitialize()
		{
			//Clear all map
			for (auto& p : systems)
			{
				p.second.reset();
			}

			keys.clear();
			
		}
	};
}
