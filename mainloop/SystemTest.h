#pragma once
//#include "EcsBase.h"
#include "SystemBase.h"
//#include "JobSystem.h";
//#include <iostream>
namespace ECS
{
	class SystemTest : public SystemBase
	{
		virtual void Update(float dt) final
		{
			/*JobSystem::Execute(
				[]() {
					std::cout << "System " << typeid(SystemTest).name() << " Executed";
				}
			);*/
		}
	};
}