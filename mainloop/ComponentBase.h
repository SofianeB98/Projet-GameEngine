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
	
}
