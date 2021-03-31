#pragma once

#include <array>
#include <iostream>
#include <queue>

#include "EcsBase.h"


namespace ECS
{

	class EntityManager
	{
		// Stock toutes les entity valide
		std::queue<Entity> available_entities;
		std::array<Entity, MAX_ENTITIES + 1> used_entities;
		// Stock les unique key des entity
		std::array<UniqueKey, MAX_ENTITIES + 1> entities_unique_key;
		// nombre d'entity actuellement en vie
		uint32_t current_living_entites;
		
	public:
		EntityManager()
		{
			this->current_living_entites = 0;

			std::fill(used_entities.begin(), used_entities.end(), NULL_ENTITY);
			
			// On initialise le tableau d'entity
			for (Entity e = 1; e < MAX_ENTITIES + 1; ++e)
				this->available_entities.push(e);
		}


		Entity CreateEntity();
		void DestroyEntity(Entity e);
		void SetUniqueKey(Entity e, UniqueKey k);
		UniqueKey GetUniqueKey(Entity e);

		const std::array<Entity, MAX_ENTITIES + 1> GetUsedEntities() const
		{
			return this->used_entities;
		}
		
		void DeInitialize()
		{
			//Clear all map
			
		}
	};

}
