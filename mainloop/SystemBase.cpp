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

		glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, .0f, -30.0f));
		view = glm::lookAt(glm::vec3(0.0f, 20.f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)800 / (float)600, 0.03f, 1000.0f);

		for (auto& e : this->entities)
		{
			const auto& rd = world.GetComponent<RendererComponent>(e);
			const auto& tr = world.GetComponent<TransformComponent>(e);
			glUseProgram(rd.program);


			glUniformMatrix4fv(glGetUniformLocation(rd.program, "model"), 1, false, glm::value_ptr(tr.transform));
			glUniformMatrix4fv(glGetUniformLocation(rd.program, "view"), 1, false, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(rd.program, "projection"), 1, false, glm::value_ptr(projection));

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



}
