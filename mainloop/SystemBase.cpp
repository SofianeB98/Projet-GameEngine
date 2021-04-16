#include <GL/glew.h>

#include "World.h"
#include "JobSystem.h"

#include "smath.h"

#include <iostream>

#include<random>
#include<cmath>
#include<chrono>

#include <set>

namespace ECS
{
	void TranslationSystem::Start(World& world)
	{
		/*int i = -50;
		int j = 0;

		int gridSize = 100;

		for (auto& e : this->entities)
		{
			auto& tr = world.GetComponent<TransformComponent>(e);
			tr.transform = glm::translate(tr.transform, { i, -3.0f, j });
			tr.transform = glm::scale(tr.transform, { 0.5f, 0.5f, 0.5f });

			j++;

			if (j % gridSize == 0)
			{
				i++;
				j = 0;
			}
		}*/

		float radius = 20.0f;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		std::uniform_real_distribution<double> uniform01(0.0, 1.0);

		for (auto& e : entities) {
			// incorrect way
			double theta = 2 * std::_Pi * uniform01(generator);
			double phi = acos(1 - 2 * uniform01(generator));

			double in_rd = radius * uniform01(generator);

			double x = sin(phi) * cos(theta) * in_rd;
			double y = sin(phi) * sin(theta) * in_rd;
			double z = cos(phi) * in_rd;

			auto& tr = world.GetComponent<TransformComponent>(e);
			tr.transform = glm::mat4(1.0f);
			tr.transform = glm::translate(tr.transform, { x, y, z });
			tr.transform = glm::scale(tr.transform, { 0.5f, 0.5f, 0.5f });
		}
	}

	void TranslationSystem::Update(float dt, World& world)
	{
		JobSystem::Execute(
			[&world, dt, this]() {
				for (auto& e : this->entities) {
					auto& translation = world.GetComponent<TransformComponent>(e);
					const auto& move = world.GetComponent<AgentFlockComponent>(e);

					translation.transform = glm::translate(translation.transform, dt * move.agent_dir);
					//const auto& move = world.GetComponent<MoveComponent>(e);

					//translation.transform = glm::translate(translation.transform, { 0.0f, dt * move.speed, 0.0f });
					//if (translation.transform[3][1] > 10.0f)
					//{
					//	const auto rd = world.GetComponent<RendererComponent>(e);

					//	auto newE = world.CreateEntity();
					//	world.AddComponent<ECS::RendererComponent>(newE,
					//		{ rd.VAO, rd.program });

					//	world.AddComponent<ECS::TransformComponent>(newE,
					//		{ });
					//	auto& trf = world.GetComponent<TransformComponent>(newE);
					//	trf.transform = translation.transform;
					//	trf.transform[3][1] = -10.0f;

					//	world.AddComponent<ECS::MoveComponent>(newE,
					//		{ move.speed * 1.025f });
					//	//translation.transform[3][1] = -10.0f;
					//	world.DestroyEntity(e);
					//}
				}
			});
		//JobSystem::Execute(
		//	[&world, dt, this]() {
		//		for (auto& e : this->entities) {
		//			auto& translation = world.GetComponent<TransformComponent>(e);
		//			const auto& move = world.GetComponent<MoveComponent>(e);

		//			translation.transform = glm::translate(translation.transform, { 0.0f, dt * move.speed, 0.0f });
		//			if (translation.transform[3][1] > 10.0f)
		//			{
		//				const auto rd = world.GetComponent<RendererComponent>(e);
		//				
		//				auto newE = world.CreateEntity();
		//				world.AddComponent<ECS::RendererComponent>(newE,
		//					{ rd.VAO, rd.program});

		//				world.AddComponent<ECS::TransformComponent>(newE,
		//					{ });
		//				auto& trf = world.GetComponent<TransformComponent>(newE);
		//				trf.transform = translation.transform;
		//				trf.transform[3][1] = -10.0f;
		//				
		//				world.AddComponent<ECS::MoveComponent>(newE,
		//					{ move.speed * 1.25f });
		//				//translation.transform[3][1] = -10.0f;
		//				world.DestroyEntity(e);
		//			}
		//		}
		//	}
		//);

		//JobSystem::Dispatch(
		//	this->entities.size(), 100,
		//	[this, &world, dt](JobDispatchArgs args)
		//	{
		//		auto it = entities.begin();
		//		std::advance(it, args.jobIndex);
		//		auto e = *it;
		//		auto& translation = world.GetComponent<TransformComponent>(e);
		//		const auto& move = world.GetComponent<MoveComponent>(e);

		//		translation.transform = glm::translate(translation.transform, { 0.0f, dt * move.speed, 0.0f });
		//		if (translation.transform[3][1] > 3.0f)
		//			translation.transform[3][1] = -3.0f;
		//	
		//		//translations.value = Add(translations.value, { 1, 1, 1, 1 });

		//	}

		// );

	}

	// =======================================================

	void RendererSystem::Start(World& world)
	{

	}

	void RendererSystem::Update(float dt, World& world)
	{
		// Update all RD
		// Entities<RendererComponent, translate...>

		for (auto& e : this->entities)
		{
			const auto& rd = world.GetComponent<RendererComponent>(e);
			const auto& tr = world.GetComponent<TransformComponent>(e);
			glUseProgram(rd.program);

			auto matloc = glGetUniformLocation(rd.program, "u_Material.AmbientColor");
			glUniform3f(matloc, rd.AmbientColor.r, rd.AmbientColor.g, rd.AmbientColor.b);

			matloc = glGetUniformLocation(rd.program, "u_Material.DiffuseColor");
			glUniform3f(matloc, rd.DiffuseColor.r, rd.DiffuseColor.g, rd.DiffuseColor.b);

			matloc = glGetUniformLocation(rd.program, "u_Material.SpecularColor");
			glUniform3f(matloc, rd.SpecularColor.r, rd.SpecularColor.g, rd.SpecularColor.b);

			matloc = glGetUniformLocation(rd.program, "u_Material.Shininess");
			glUniform1f(matloc, rd.Shininess);

			matloc = glGetUniformLocation(rd.program, "u_Material.Metalness");
			glUniform1f(matloc, rd.Metalness);

			glUniformMatrix4fv(glGetUniformLocation(rd.program, "model"), 1, false, glm::value_ptr(tr.transform));
			
			/*glUniformMatrix4fv(glGetUniformLocation(rd.program, "view"), 1, false, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(rd.program, "projection"), 1, false, glm::value_ptr(projection));

			glUniform3f(glGetUniformLocation(rd.program, "u_viewPos"), view[3][0], view[3][1], view[3][2]);*/
			
			glBindVertexArray(rd.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	// =======================================================

	glm::vec3 FlockSystem::GetStayInRadiusBehavior(const glm::vec3& entity_position)
	{
		auto offset = this->flock_center - entity_position;
		float length = glm::length(offset);
		float t = length / this->flock_radius;
		if (t < 0.9f)
			return { 0.0f, 0.0f, 0.0f };

		return t * t * offset;
	}

	glm::vec3 FlockSystem::GetAlignementBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_direction, World& world)
	{
		if (context.empty())
			return glm::normalize(current_agent_direction);

		glm::vec3 alignement = { 0.0f, 0.0f,0.0f };
		for (auto& e : context)
		{
			const auto& e_agent = world.GetComponent<AgentFlockComponent>(e);
			const auto e_dir = e_agent.agent_dir;
			alignement += glm::normalize(e_dir);
		}

		return alignement * (1.0f / context.size());
	}

	glm::vec3 FlockSystem::GetAvoidanceBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_position, World& world)
	{
		glm::vec3 avoidance = { 0.0f, 0.0f,0.0f };
		if (context.empty())
			return avoidance;

		int n_avoid = 0;
		for (auto& e : context)
		{
			const auto& e_transform = world.GetComponent<TransformComponent>(e);
			const auto e_pos = glm::vec3(e_transform.transform[3][0], e_transform.transform[3][1], e_transform.transform[3][2]);
			auto offset = e_pos - current_agent_position;

			if (glm::dot(offset, offset) >= flock_squared_avoidance_radius)
				continue;

			n_avoid++;
			avoidance += (current_agent_position - e_pos);
		}

		if (n_avoid > 0)
			avoidance /= context.size();

		return avoidance;
	}

	glm::vec3 FlockSystem::GetCohesionBehavior(const std::vector<Entity>& context, const glm::vec3& current_agent_position, World& world)
	{
		glm::vec3 cohesion = { 0.0f, 0.0f,0.0f };
		if (context.empty())
			return cohesion;

		for (auto& e : context)
		{
			const auto& e_transform = world.GetComponent<TransformComponent>(e);
			const auto e_pos = glm::vec3(e_transform.transform[3][0], e_transform.transform[3][1], e_transform.transform[3][2]);

			cohesion += e_pos;
		}
		cohesion /= context.size();

		cohesion -= current_agent_position;

		return cohesion;
	}

	std::vector<Entity> FlockSystem::GetContext(const Entity ignore_entity, const glm::vec3& entity_pos, World& world)
	{
		context.clear();
		/*JobSystem::Execute(
			[&world, ignore_entity, entity_pos, this]() {*/
				for (auto& e : entities)
				{
					if (e == ignore_entity)
						continue;

					const auto& e_transform = world.GetComponent<TransformComponent>(e);
					const auto e_pos = glm::vec3(e_transform.transform[3][0], e_transform.transform[3][1], e_transform.transform[3][2]);

					auto offset = e_pos - entity_pos;
					if (glm::dot(offset, offset) <= flock_squared_neighbor_radius + 0.25f)
						context.push_back(e);
				}
			//});

		return context;
	}


	void FlockSystem::Start(World& world)
	{
		context.reserve(MAX_ENTITIES + 1);

		flock_squared_neighbor_radius = flock_neighbor_radius * flock_neighbor_radius;
		flock_squared_avoidance_radius = flock_neighbor_radius * flock_avoidance_radius_multiplier * flock_avoidance_radius_multiplier;
	}

	void FlockSystem::Update(float dt, World& world)
	{
		// Pour chaque entity :
		// Je calcule les vecteurs suivant :
		//		=> Alignement
		//		=> Avoidance
		//		=> Cohesion
		//		=> Stay in radius
		/*JobSystem::Execute(
			[&world, dt, this]() {*/
		for (auto& e : entities)
		{

			const auto& e_transform = world.GetComponent<TransformComponent>(e);
			const auto e_pos = glm::vec3(e_transform.transform[3][0], e_transform.transform[3][1], e_transform.transform[3][2]);
			auto& e_agent = world.GetComponent<AgentFlockComponent>(e);

			auto context = GetContext(e, e_pos, world);

			/*
			 *
			 *  if (partialMove != Vector2.zero)
			{
				if (partialMove.sqrMagnitude > weights[i] * weights[i])
				{
					partialMove.Normalize();
					partialMove *= weights[i];
				}

				move += partialMove;
			}
			 */

			auto alignement = GetAlignementBehavior(context, e_agent.agent_dir, world) * alignement_WEIGHT;
			if (glm::length(alignement) > 0.01f)
			{
				if (glm::dot(alignement, alignement) > alignement_WEIGHT * alignement_WEIGHT)
				{
					alignement = glm::normalize(alignement) * alignement_WEIGHT;
				}
			}
			auto avoidance = GetAvoidanceBehavior(context, e_pos, world) * avoidance_WEIGHT;
			if (glm::length(avoidance) > 0.01f)
			{
				if (glm::dot(avoidance, avoidance) > avoidance_WEIGHT * avoidance_WEIGHT)
				{
					avoidance = glm::normalize(avoidance) * avoidance_WEIGHT;
				}
			}
			auto cohesion = GetCohesionBehavior(context, e_pos, world) * cohesion_WEIGHT;
			if (glm::length(cohesion) > 0.01f)
			{
				if (glm::dot(cohesion, cohesion) > cohesion_WEIGHT * cohesion_WEIGHT)
				{
					cohesion = glm::normalize(cohesion) * cohesion_WEIGHT;
				}
			}
			auto stay_in_radius = GetStayInRadiusBehavior(e_pos) * stay_in_radius_WEIGHT;
			if (glm::length(stay_in_radius) > 0.01f)
			{
				if (glm::dot(stay_in_radius, stay_in_radius) > stay_in_radius_WEIGHT * stay_in_radius_WEIGHT)
				{
					stay_in_radius = glm::normalize(stay_in_radius) * stay_in_radius_WEIGHT;
				}
			}


			e_agent.agent_dir = (alignement + avoidance + cohesion + stay_in_radius) * e_agent.drive_factor;

			if (glm::length(e_agent.agent_dir) > e_agent.max_speed)
			{
				e_agent.agent_dir = glm::normalize(e_agent.agent_dir) * e_agent.max_speed;
			}
		}
		/*});*/
	}

	// =======================================================

	void LifeTimeSystem::Start(World& world)
	{
		
	}

	void LifeTimeSystem::Update(float dt, World& world)
	{
		for (auto& e : entities)
		{
			auto& ltc = world.GetComponent<LifeTimeComponent>(e);
			ltc.life -= dt * ltc.speed_lose_life;

			float ratio = ltc.life / ltc.start_life;

			if(ratio <= 0.25f)
			{
				float realRatio = ratio / 0.25f;
				auto& rd = world.GetComponent<RendererComponent>(e);
				rd.DiffuseColor.r *= realRatio;
				rd.DiffuseColor.g *= realRatio;
				rd.DiffuseColor.b *= realRatio;

				rd.AmbientColor.r *= realRatio;
				rd.AmbientColor.g *= realRatio;
				rd.AmbientColor.b *= realRatio;
			}
			
			if (ltc.life < 0.1f)
				world.DestroyEntity(e);
		}
	}

	// =======================================================

	void FlockAgentSpawnerSystem::Start(World& world)
	{
		
	}

	void FlockAgentSpawnerSystem::Update(float dt, World& world)
	{	
		if (world.GetLivingEntities() >= MAX_ENTITIES)
			return;

		if(current_delay < spawn_delay)
		{
			current_delay += dt;
			//std::cout << current_delay << std::endl;
			return;
		}

		current_delay -= spawn_delay;
		
		float radius = 20.0f;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		std::uniform_real_distribution<double> uniform01(0.0, 1.0);

		uint32_t entityToCreate = MAX_ENTITIES - world.GetLivingEntities();
		entityToCreate = entityToCreate > max_entities_to_spawn ? max_entities_to_spawn : entityToCreate;
		
		Entity refEntity = world.FindEntityWith<RendererComponent>();
		if (refEntity == NULL_ENTITY)
			return;
		
		const auto& rdC = world.GetComponent<RendererComponent>(refEntity);
		
		for (uint32_t i = 0; i < entityToCreate; ++i)
		{
			Entity ent = world.CreateEntity();
			world.AddComponent<ECS::RendererComponent>(ent,
				{rdC.VAO, rdC.program});

			auto& rd = world.GetComponent<RendererComponent>(ent);
			rd.AmbientColor = glm::vec3(uniform01(generator), uniform01(generator), uniform01(generator));
			rd.DiffuseColor = glm::vec3(uniform01(generator), uniform01(generator), uniform01(generator));
			
			world.AddComponent<ECS::TransformComponent>(ent,
				{ });
			
			// incorrect way
			double theta = 2 * std::_Pi * uniform01(generator);
			double phi = acos(1 - 2 * uniform01(generator));

			double in_rd = radius * uniform01(generator);

			double x = sin(phi) * cos(theta) * in_rd;
			double y = sin(phi) * sin(theta) * in_rd;
			double z = cos(phi) * in_rd;

			auto& tr = world.GetComponent<TransformComponent>(ent);
			tr.transform = glm::mat4(1.0f);
			tr.transform = glm::translate(tr.transform, { x, y, z });
			tr.transform = glm::scale(tr.transform, { 0.66f, 0.66f, 0.66f });
			
			world.AddComponent<ECS::AgentFlockComponent>(ent,
				{ });
			auto& afc = world.GetComponent<AgentFlockComponent>(ent);
			//afc.max_speed = uniform01(generator) * 20.0f;
			//afc.drive_factor = uniform01(generator) * 50.0f;
			afc.agent_dir = glm::normalize(glm::vec3(uniform01(generator), uniform01(generator), uniform01(generator)));
			world.AddComponent<ECS::LifeTimeComponent>(ent,
				{ (float)uniform01(generator) * 50.0f + 0.5f, (float)uniform01(generator) * 8.0f + 1.0f });
		}

		std::cout << "Created !! Current living entities : " << world.GetLivingEntities() << std::endl;
		
	}

	// =======================================================
}

/*
 *
 * float radius = 20.0f;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		std::uniform_real_distribution<double> uniform01(0.0, 1.0);

			// incorrect way
			double theta = 2 * std::_Pi * uniform01(generator);
			double phi = acos(1 - 2 * uniform01(generator));

			double in_rd = radius * uniform01(generator);

			double x = sin(phi) * cos(theta) * in_rd;
			double y = sin(phi) * sin(theta) * in_rd;
			double z = cos(phi) * in_rd;

			auto& tr = world.GetComponent<TransformComponent>(e);
			tr.transform = glm::mat4(1.0f);
			tr.transform = glm::translate(tr.transform, { x, y, z });
			tr.transform = glm::scale(tr.transform, { 0.5f, 0.5f, 0.5f });
		
 */