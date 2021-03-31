#pragma once

namespace ECS
{

	struct ComponentBase
	{
		
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
