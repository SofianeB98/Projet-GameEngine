#include "EntityManager.h"

#include <cassert>


ECS::Entity ECS::EntityManager::CreateEntity()
{
	assert(this->current_living_entites < MAX_ENTITIES, "Trop d'entity existante !");

	Entity e = available_entities.front();
	available_entities.pop();
	++current_living_entites;

	this->used_entities[e] = e;
	
	return e;
}

void ECS::EntityManager::DestroyEntity(Entity e)
{
	assert(e < MAX_ENTITIES, "Entity inexistante");

	this->entities_unique_key[e].reset();
	
	this->used_entities[e] = NULL_ENTITY;

	this->available_entities.push(e);
	--this->current_living_entites;
}

void ECS::EntityManager::SetUniqueKey(Entity e, UniqueKey k)
{
	assert(e < MAX_ENTITIES, "Entity inexistante");

	this->entities_unique_key[e] = k;
}

ECS::UniqueKey ECS::EntityManager::GetUniqueKey(Entity e)
{
	assert(e < MAX_ENTITIES, "Entity inexistante");

	return this->entities_unique_key[e];
}
