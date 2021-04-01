#include <GL/glew.h>

#include "World.h"
#include "JobSystem.h"

#include "smath.h"

#include <iostream>

#include <set>

void ECS::TranslationSystem::Start(const World& world)
{
	int i = -50;
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
	}
}


void ECS::TranslationSystem::Update(float dt, const World& world)
{
	/*for (auto& e : this->entities) {
		auto& translation = world.GetComponent<TransformComponent>(e);
		const auto& move = world.GetComponent<MoveComponent>(e);

		translation.transform = glm::translate(translation.transform, { 0.0f, dt * move.speed, 0.0f });
		if (translation.transform[3][1] > 3.0f)
			translation.transform[3][1] = -3.0f;
	}*/

	JobSystem::Execute(
		[&world, dt, this]() {
			for (auto& e : this->entities) {
				auto& translation = world.GetComponent<TransformComponent>(e);
				const auto& move = world.GetComponent<MoveComponent>(e);

				translation.transform = glm::translate(translation.transform, { 0.0f, dt * move.speed, 0.0f });
				if (translation.transform[3][1] > 10.0f)
					translation.transform[3][1] = -10.0f;
			}
		}
	);

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

void ECS::RendererSystem::Start(const World& world)
{

}


void ECS::RendererSystem::Update(float dt, const World& world)
{
	// Update all RD
	// Entities<RendererComponent, translate...>

	glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, .0f, -30.0f));
	view = glm::lookAt(glm::vec3(50.0f, 14.0f, sqrt(50.0f * 50.0f * 2.0f ) + 30.0f ), glm::vec3(0.0f, 0.0f, 50.0f ), glm::vec3(0.0f, 1.0f, 0.0f ));
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)800 / (float)600, 0.03f, 1000.0f);

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
