#pragma once

namespace ECS
{

	struct ComponentBase
	{
		bool isNullComponent = true;
	};
	
	struct TranslationComponent : ComponentBase
	{
		float a;
	};

	struct RotationComponent : ComponentBase
	{
		float b;

	};
	
}
