#pragma once
#include <memory>
#include <unordered_map>


#include "fnv.h"
#include "IComponentArray.h"

namespace ECS
{
	class ComponentManager
	{
		std::unordered_map</*const char**/uint32_t, ComponentType> component_types;
		std::unordered_map</*const char**/uint32_t, std::shared_ptr<IComponentArray>> component_arrays;
		ComponentType next_component_type;

		template <typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			//const char* typeName = typeid(T).name();
			uint32_t typeName = FNV::fnv1a(typeid(T).name());

			assert(this->component_types.find(typeName) != component_types.end(), "Ce component array existe po");

			return std::static_pointer_cast<ComponentArray<T>>(this->component_arrays[typeName]);
		}

	public:
		template <typename T>
		void RegisterComponent()
		{
			//const char* typeName = typeid(T).name();
			uint32_t typeName = FNV::fnv1a(typeid(T).name());

			assert(this->component_types.find(typeName) == this->component_types.end(), "Ce component array existe deja !");

			this->component_types.insert({
				typeName,
				next_component_type
				});

			this->component_arrays.insert({
				typeName,
				std::make_shared<ComponentArray<T>>()
				});

			// Subscribe la fonction EntityDestroy du array a un event
			
			++next_component_type;
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			//const char* typeName = typeid(T).name();
			uint32_t typeName = FNV::fnv1a(typeid(T).name());
			
			assert(this->component_types.find(typeName) != component_types.end(), "Ce component array existe po");

			return this->component_types[typeName];
		}

		template <typename T>
		void AddComponent(Entity e, T component)
		{

			GetComponentArray<T>()->InsertData(e, component);
		}

		template <typename T>
		void RemoveComponent(Entity e)
		{
			GetComponentArray<T>()->RemoveData(e);
		}

		template <typename T>
		T& GetComponent(Entity e)
		{
			return GetComponentArray<T>()->GetData(e);
		}

		void EntityDestroyed(Entity e)
		{
			//Invoquer un event
		}
	};
}
