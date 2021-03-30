#pragma once
#include <memory>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace ECS
{

	class World
	{
		std::unique_ptr<EntityManager> entity_manager;
		std::unique_ptr<ComponentManager> component_manager;
		std::unique_ptr<SystemManager> system_manager;

	public:
		World()
		{
			Init();
		}
		
		void Init()
		{
			this->entity_manager = std::make_unique<EntityManager>();
			this->component_manager = std::make_unique<ComponentManager>();
			this->system_manager = std::make_unique<SystemManager>();
		}

#pragma region Entity
		Entity CreateEntity()
		{
			return this->entity_manager->CreateEntity();
		}

		void DestroyEntity(Entity e)
		{
			this->entity_manager->DestroyEntity(e);
			this->component_manager->EntityDestroyed(e);
			this->system_manager->EntityDestroyed(e);
		}
#pragma endregion 

		
#pragma region Component
		template <typename T>
		void RegisterComponent()
		{
			this->component_manager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity e, T c)
		{
			this->component_manager->AddComponent<T>(e, c);

			UniqueKey k = entity_manager->GetUniqueKey(e);
			k.set(component_manager->GetComponentType<T>(), true);
			entity_manager->SetUniqueKey(e, k);

			system_manager->EntityKeyChanged(e, k);
		}

		template <typename T>
		void RemoveComponent(Entity e)
		{
			this->component_manager->RemoveComponent<T>(e);

			UniqueKey k = entity_manager->GetUniqueKey(e);
			k.set(component_manager->GetComponentType<T>(), false);
			entity_manager->SetUniqueKey(e, k);

			system_manager->EntityKeyChanged(e, k);
		}

		template <typename T>
		T& GetComponent(Entity e)
		{
			return component_manager->GetComponent<T>(e);
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			return this->component_manager->GetComponentType<T>();
		}
#pragma endregion

		
#pragma region System
		template<typename T>
		UniqueKey CreateSystemUniqueKey()
		{
			static UniqueKey k;
			return k;
		}
		
		template<typename T, typename... C>
		UniqueKey CreateSystemUniqueKey(T value, C... c)
		{
			static UniqueKey k;
			k.set(GetComponentType<T>(), true);
			return CreateSystemUniqueKey<T>(c...);
		}
		
		template <typename T, typename ...C>
		std::shared_ptr<T> RegisterSystem(C ...c)
		{

			std::cout << "Je commence a cree la key ! " << std::endl;
			UniqueKey key;

			key = CreateSystemUniqueKey(c...);

			
			SetSystemUniqueKey<T>(key);
			
			//auto sys = this->system_manager->RegisterSystem<T>();
			return nullptr;
		}

		
		template <typename T>
		void SetSystemUniqueKey(UniqueKey k)
		{
			this->system_manager->SetUniqueKey<T>(k);
		}
#pragma endregion 
		
	};
}
