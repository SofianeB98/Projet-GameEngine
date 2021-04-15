#pragma once
#include "smath.h"
namespace ECS
{

	struct ComponentBase
	{
		bool isNullComponent = true;
	};
	
	struct TransformComponent : ComponentBase
	{
		//float4 value;
		glm::mat4 transform = glm::mat4(1.0f);
		
	};

	struct MoveComponent : ComponentBase
	{
		float speed = 2.5f;

		MoveComponent() = default;
		MoveComponent(float sp) : speed(sp) {}
	};
	
	struct RendererComponent : ComponentBase
	{
		uint32_t VAO;

		// Pour du test, nous implementons les valeur de material ici
		// L'ideal serait de créer une class/struct material afin de partager une instance d'un
		// même material pour plusieurs renderer
		glm::vec3 AmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 DiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 SpecularColor = glm::vec3(0.75f, 0.75f, 0.75f);
		float Shininess = 0.5f;
		float Metalness = 0.1f;
		
		uint32_t program;

		RendererComponent() = default;
		RendererComponent(uint32_t vao, uint32_t prog) : VAO(vao), program(prog){}
	};

	struct LifeTimeComponent : ComponentBase
	{
		float start_life = 10.0f;
		float life = 10.0f;
		float speed_lose_life = 2.0f;

		LifeTimeComponent() = default;
		LifeTimeComponent(float start_life, float speed_lose_life) : start_life(start_life), life(start_life), speed_lose_life(speed_lose_life){}
	};
	
	struct AgentFlockComponent : ComponentBase
	{
		glm::vec3 agent_dir = glm::vec3(0.0f, 0.0f, 1.0f);
		float drive_factor = 20.0f;
		float max_speed = 10.0f;
	};
	
}
