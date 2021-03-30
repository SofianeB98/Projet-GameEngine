#pragma once
#include "EcsBase.h"
#include "SystemBase.h"
#include "JobSystem.h";
#include <iostream>
namespace Tests
{
	class SystemTest : ECS::SystemBase
	{
		virtual void Update()
		{
			JobSystem::Execute(
				[]() {
					std::cout << "System " << typeid(SystemTest).name() << " Executed";
				}
			);
		}
	};
}