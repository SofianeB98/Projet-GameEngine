#include "World.h";

void ECS::SystemTest::Update(float dt, World& world)
{
	JobSystem::Execute(
		[&world, dt, this]() {
			std::cout << "System " << typeid(SystemTest).name() << " Executed";
		
		}
	);

}
