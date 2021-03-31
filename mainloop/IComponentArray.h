#pragma once
#include <cassert>
#include <unordered_map>


#include "ComponentBase.h"
#include "EcsBase.h"

namespace ECS
{

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity e) = 0;
	};

	template <typename T = ComponentBase>
	class ComponentArray : public IComponentArray
	{
		std::array<T, MAX_ENTITIES + 1> component_array;
		std::unordered_map<Entity, size_t> entity_to_indexMap;
		std::unordered_map<size_t, Entity> index_to_entityMap;

		size_t size = 1;
		
	public:
		void InsertData(Entity e, T component)
		{
			assert(this->entity_to_indexMap.find(e) == this->entity_to_indexMap.end(), "Component deja ajoute");

			//assert(this->size < MAX_ENTITIES);
			component.isNullComponent = false;
			
			size_t idx = this->size;
			this->entity_to_indexMap[e] = idx;
			this->index_to_entityMap[idx] = e;
			this->component_array[idx] = component;

			++this->size;
		}

		void RemoveData(Entity e)
		{
			assert(this->entity_to_indexMap.find(e) != this->entity_to_indexMap.end(), "Cette entity n'a pas ce component");

			// Je prend l'index map qui correspond a l'entity e
			size_t removed_entity_idx = this->entity_to_indexMap[e];
			// Je prend le dernier element possible
			size_t last_element_index = this->size - 1;
			// je remplace le component de m'enity removed par celui du last idx
			this->component_array[removed_entity_idx] = this->component_array[last_element_index];
			//this->component_array[last_element_index] = this->component_array[0];

			// je recupere l'entity du last element
			Entity entity_of_last_element = this->index_to_entityMap[last_element_index];
			// je set la derniere entity en tant que removed entity
			this->entity_to_indexMap[entity_of_last_element] = removed_entity_idx;
			// je set l'index qui me donne l'entity, du remove a celle du last element
			this->index_to_entityMap[removed_entity_idx] = entity_of_last_element;

			// On supprime
			this->entity_to_indexMap.erase(e);
			this->index_to_entityMap.erase(last_element_index);

			// on diminue la taille actuelle
			--this->size;
		}

		T& GetData(Entity e)
		{
			//assert(this->entity_to_indexMap.find(e) != this->entity_to_indexMap.end(), "Cette entity n'a pas ce component");
			if (this->entity_to_indexMap.find(e) == this->entity_to_indexMap.end())
				return this->component_array[0];
			
			return this->component_array[this->entity_to_indexMap[e]];
		}

		void EntityDestroyed(Entity e) override
		{
			if (this->entity_to_indexMap.find(e) != this->entity_to_indexMap.end())
				RemoveData(e);
		}
	};
}
