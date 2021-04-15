#include "World.h"

void ECS::World::Initialize()
{
	this->entity_manager = std::make_unique<EntityManager>();
	this->component_manager = std::make_unique<ComponentManager>();
	this->system_manager = std::make_unique<SystemManager>();

	// Le component manager va register tout les component deja connu
	this->component_manager->Initialize();

	// Ensuite on va register tout les System connu
	this->RegisterSystem<ECS::FlockSystem>(ECS::TransformComponent(), ECS::AgentFlockComponent());
	this->RegisterSystem<ECS::TranslationSystem>(ECS::TransformComponent(), ECS::AgentFlockComponent());
	this->RegisterSystem<ECS::LifeTimeSystem>(ECS::LifeTimeComponent(), ECS::RendererComponent());
	this->RegisterSystem<ECS::RendererSystem>(ECS::RendererComponent(), ECS::TransformComponent());
}


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
