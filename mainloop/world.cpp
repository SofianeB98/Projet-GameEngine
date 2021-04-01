#include "World.h"

void ECS::World::DeInitialize()
{
	// DeInitialize all
	this->system_manager->DeInitialize();
	this->component_manager->DeInitialize();
	this->entity_manager->DeInitialize();

	// Delete all
	delete system_manager.release();
	delete component_manager.release();
	delete entity_manager.release();
}
