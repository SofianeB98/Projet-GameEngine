#pragma once
#include<set>
#include "EcsBase.h"
namespace ECS {

	class SystemBase
	{
	public:
		std::set<Entity> entities;
		// Events
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		// Update Loop
		virtual void Update(double dt) = 0;
	};
}

