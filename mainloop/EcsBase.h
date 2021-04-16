#pragma once
#include <bitset>
#include <cstdint>

namespace ECS
{
	// Defini une entity
	// En ecs, une entity est juste un ID
	// de 0 à max
	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 10000;
	const Entity NULL_ENTITY = 0;

	// Defini l'id d'un component
	// De 0 à Max
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 10;

	// Utilise pour savoir quels sont les components associes à une Entity
	using UniqueKey = std::bitset<MAX_COMPONENTS>;
}
