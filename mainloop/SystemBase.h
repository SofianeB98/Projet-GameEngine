#pragma once
#include<set>

#include "EcsBase.h"


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
		virtual void Start(const World& world) = 0;
		virtual void Update(float dt, const World& world) = 0;
	};

	class TranslationSystem : public SystemBase
	{
		virtual void Start(const World& world) final;
		virtual void Update(float dt, const World& world) final;
	};

	class RendererSystem : public SystemBase
	{
		virtual void Start(const World& world) final;
		virtual void Update(float dt, const World& world) final;
	};
}

