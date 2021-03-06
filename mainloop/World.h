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

		std::vector<Entity> entity_to_destroy;

	public:
		void Initialize();

		void Start()
		{
			system_manager->StartSystem(*this);
		}

		void Update(float deltaTime)
		{
			system_manager->UpdateSystem(deltaTime, *this);
		}

		void PostUpdate()
		{
			for (auto e : entity_to_destroy)
			{
				this->entity_manager->DestroyEntity(e);
				this->component_manager->EntityDestroyed(e);
				this->system_manager->EntityDestroyed(e);
			}
			entity_to_destroy.clear();
		}

		void DeInitialize();

#pragma region Entity
		Entity CreateEntity() const
		{
			return this->entity_manager->CreateEntity();
		}

		void DestroyEntity(Entity e)
		{
			if(std::find(entity_to_destroy.begin(), entity_to_destroy.end(), e) == entity_to_destroy.end())
				entity_to_destroy.push_back(e);
		}

		uint32_t GetLivingEntities() const
		{
			return this->entity_manager->GetLivingEntities();
		}
#pragma endregion 


#pragma region Component
		template <typename T>
		void RegisterComponent()
		{
			this->component_manager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity e, T c) const
		{
			this->component_manager->AddComponent<T>(e, c);

			UniqueKey k = entity_manager->GetUniqueKey(e);
			k.set(component_manager->GetComponentType<T>(), true);
			entity_manager->SetUniqueKey(e, k);

			system_manager->EntityKeyChanged(e, k);
		}

		template <typename T>
		void RemoveComponent(Entity e) const
		{
			this->component_manager->RemoveComponent<T>(e);

			UniqueKey k = entity_manager->GetUniqueKey(e);
			k.set(component_manager->GetComponentType<T>(), false);
			entity_manager->SetUniqueKey(e, k);

			system_manager->EntityKeyChanged(e, k);
		}

		template <typename T>
		T& GetComponent(Entity e) const
		{
			return component_manager->GetComponent<T>(e);
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			return this->component_manager->GetComponentType<T>();
		}

		template <typename T = ComponentBase>
		bool HasComponent(Entity e) const
		{
			return !GetComponent<T>(e).isNullComponent;
		}

		template <typename T = ComponentBase>
		std::vector<Entity> FindEntitiesWith() const
		{
			//do some stuff
			std::vector<Entity> entitiesWithT;
			entitiesWithT.reserve(MAX_ENTITIES + 1);

			auto used_entities = this->entity_manager->GetUsedEntities();

			for (auto it = used_entities.begin(); it != used_entities.end(); ++it)
			{
				if (*it == NULL_ENTITY)
					continue;

				if (HasComponent<T>(*it))
					entitiesWithT.push_back(*it);
			}


			return entitiesWithT;
		}

		template <typename T = ComponentBase>
		Entity FindEntityWith() const
		{
			//do some stuff
			std::vector<Entity> entitiesWithT;
			entitiesWithT.reserve(MAX_ENTITIES + 1);

			auto used_entities = this->entity_manager->GetUsedEntities();

			for (auto it = used_entities.begin(); it != used_entities.end(); ++it)
			{
				if (*it == NULL_ENTITY)
					continue;

				if (HasComponent<T>(*it))
					return *it;
			}


			return NULL_ENTITY;
		}
#pragma endregion


#pragma region System
	private:
		template <typename T>
		void SetSystemUniqueKey(UniqueKey k)
		{
			this->system_manager->SetUniqueKey<T>(k);
		}

		UniqueKey SetUniqueKey(UniqueKey& k)
		{
			// RIEN
			std::cout << "Unique key created" << std::endl;
			return k;
		}

		template<typename T, typename... C>
		UniqueKey SetUniqueKey(UniqueKey& k, T value, C... c)
		{
			std::cout << "Requierement : " << typeid(value).name() << std::endl;

			k.set(GetComponentType<T>(), true);

			return SetUniqueKey(k, c...);
		}


	public:
		template <typename T, typename U, typename ...C>
		std::shared_ptr<T> RegisterSystem(U value, C ...c)
		{
			auto sys = this->system_manager->RegisterSystem<T>();

			std::cout << "Je commence a cree la key du system " << typeid(T).name() << std::endl;
			UniqueKey key;
			key = SetUniqueKey(key, value, c...);


			SetSystemUniqueKey<T>(key);

			return sys;
		}


#pragma endregion 

	};
}
