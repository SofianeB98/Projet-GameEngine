#include <GL/glew.h>

#include "World.h"
#include "JobSystem.h"

#include "smath.h"

#include <iostream>

#include <set>


void ECS::SystemTest::Update(float dt, const World& world)
{
	//JobSystem::Execute(
	//	[&world, dt, this]() {
	//		//std::cout << "System " << typeid(SystemTest).name() << " Executed";
	//		for (auto& e : this->entities) {
	//			auto& translations = world.GetComponent<TranslationComponent>(e);
	//			translations.value = Add(translations.value, { 1, 1, 1, 1 });
	//			std::cout << "Translation now : " << translations.value.a[0] << std::endl;
	//		}
	//	}
	//);

	JobSystem::Dispatch(
		this->entities.size(), 16,
		[this, &world, dt](JobDispatchArgs args)
		{
			auto it = entities.begin();
			std::advance(it, args.jobIndex);
			auto e = *it;
			auto& translations = world.GetComponent<TranslationComponent>(e);
			translations.value = Add(translations.value, { 1, 1, 1, 1 });
			
		}

	);

}


void ECS::RendererSystem::Update(float dt, const World& world)
{
	// Update all RD
	// Entities<RendererComponent, translate...>

	for (auto& e : this->entities) 
	{
		std::cout << "Render system !! " << std::endl;
		
		auto rd = world.GetComponent<RendererComponent>(e);
		glUseProgram(rd.program);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { 2.0f,  5.0f, -15.0f });
		glm::mat4 view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
		glBindVertexArray(rd.VAO);

		glUniformMatrix4fv(glGetUniformLocation(rd.program, "model"), 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(rd.program, "view"), 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(rd.program, "projection"), 1, false, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	
	
	
}
