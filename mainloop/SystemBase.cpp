#include "World.h";
#include "smath.h"
#include <iostream>
#include <set>
#include <valarray>
void ECS::SystemTest::Update(float dt, const World& world)
{
	//JobSystem::Execute(
	//	[&world, dt, this]() {
	//		//std::cout << "System " << typeid(SystemTest).name() << " Executed";
	//		for (auto& e : this->entities) {
	//			auto& translations = world.GetComponent<TranslationComponent>(e);
	//			translations.value = Add(translations.value, { 1, 1, 1, 1 });
	//			std::cout << "Translation now : " << translations.value.a[0] << std::endl;
	//		}
	//	}
	//);

	JobSystem::Dispatch(
		this->entities.size(), 16,
		[this, &world, dt](JobDispatchArgs args)
		{
			auto it = entities.begin();
			std::advance(it, args.jobIndex);
			auto e = *it;
			auto& translations = world.GetComponent<TranslationComponent>(e);
			translations.value = Add(translations.value, { 1, 1, 1, 1 });
			
		}

	);

}
