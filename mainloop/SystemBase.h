#pragma once
#include<set>

#include "ComponentBase.h"
#include "EcsBase.h"
#include "JobSystem.h"

namespace ECS {
	class World;

	class SystemBase
	{
	public:
		std::set<Entity> entities;
		// Events
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		// Update Loop
		virtual void Update(float dt, const World& world) = 0;
	};

	class SystemTest : public SystemBase
	{	
		virtual void Update(float dt, const World& world) final;
	};
}

