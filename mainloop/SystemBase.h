#pragma once
#include<set>

#include "EcsBase.h"


namespace ECS {
	class World;

	class SystemBase
	{
	public:
		std::set<Entity> entities;
		// Events
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		// Update Loop
		virtual void Start( World& world) = 0;
		virtual void Update(float dt,  World& world) = 0;
	};

	class TranslationSystem : public SystemBase
	{
		virtual void Start( World& world) final;
		virtual void Update(float dt,  World& world) final;
	};

	class RendererSystem : public SystemBase
	{
		virtual void Start( World& world) final;
		virtual void Update(float dt,  World& world) final;
	};

	class FlockSystem : public SystemBase
	{
		float flock_radius = 20.0f;
		glm::vec3 flock_center = glm::vec3(0.0f, 0.0f, 0.0f);

		float flock_neighbor_radius = 1.50f;
		float flock_squared_neighbor_radius = 1.0f;

		float flock_avoidance_radius_multiplier = 0.5f;
		float flock_squared_avoidance_radius = 0.5f;

		float stay_in_radius_WEIGHT = 3.0f;
		float alignement_WEIGHT = 2.0f;
		float avoidance_WEIGHT = 8.0f;
		float cohesion_WEIGHT = 3.0f;
		
		std::vector<Entity> context;
		
		glm::vec3 GetStayInRadiusBehavior(const glm::vec3& entity_position);
		glm::vec3 GetAlignementBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_direction, World& world);
		glm::vec3 GetAvoidanceBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_position, World& world);
		glm::vec3 GetCohesionBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_position, World& world);

		std::vector<Entity> GetContext(const Entity ignore_entity, const glm::vec3& entity_pos, World& world);
		
	public:
		virtual void Start(World& world) final;
		virtual void Update(float dt, World& world) final;
	};

	class FlockAgentSpawnerSystem : public SystemBase
	{
		
	};
	
	class LifeTimeSystem : public SystemBase
	{
	public:
		void Start(World& world) final;
		void Update(float dt, World& world) final;
	};
}

