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

		uint32_t program;

		RendererComponent() = default;
		RendererComponent(uint32_t vao, uint32_t prog) : VAO(vao), program(prog){}
	};

	struct AgentFlockComponent : ComponentBase
	{
		glm::vec3 agent_dir = glm::vec3(0.0f, 0.0f, 1.0f);
		float drive_factor = 20.0f;
		float max_speed = 10.0f;
	};
	
}
