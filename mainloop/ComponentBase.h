#pragma once
#include "smath.h"
namespace ECS
{

	struct ComponentBase
	{
		bool isNullComponent = true;
	};
	
	struct TranslationComponent : ComponentBase
	{
		float4 value;
	};

	struct RotationComponent : ComponentBase
	{
		float b;
	};

	struct RendererComponent : ComponentBase
	{
		uint32_t VBO;
		uint32_t VAO;

		uint32_t program;

		RendererComponent() = default;
		RendererComponent(uint32_t vao, uint32_t vbo, uint32_t prog) : VAO(vao), VBO(vbo), program(prog){}
	};
	
}
