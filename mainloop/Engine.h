#pragma once

#include "InfraStructure.h"
#include "World.h"

namespace ESGI
{
	// "forward declaration" d'une structure qui elle meme reference des classes
	// qui ne sont pas encore definies (fonctionne ici car il s'agit de pointeurs,
	// seul le typage interesse le compilateur dans ce cas)	
	struct EngineContext;
	
	
	struct Engine : public InfraStructure
	{
		// Un exemple basique de sous-systeme du moteur (par ex. le moteur Behaviour)
		struct EngineSubSystem
		{
			ECS::World* world;

			double targetFrameRate = 1.0 / 60.0;
			double accumulatedTime = 0.0;
			int maxIterations = 2;

			bool Create() { return true; }
			void Destroy() {}
			bool Initialize()
			{
				world = new ECS::World;
				world->Initialize();

				return true;
			}
			
			void DeInitialize()
			{
				world->DeInitialize();
				delete world;
			}

			void Start()
			{
				world->Start();
			}
			
			void Update(float deltaTime)
			{
				world->Update(deltaTime);
			}
			
			void FixedUpdate(float deltaTime)
			{
				
			}

			void PostUpdate()
			{
				world->PostUpdate();
			}
		};
		
		// simple exemple, ou alors un vector ou une liste simplement chainee (intrusive) de systems
		EngineSubSystem* m_AIEngine;

		void ProcessSystems(double elapsedTime);

		// ---

		bool Initialize() final;

		void DeInitialize() final;

		void Start()
		{
			this->m_AIEngine->Start();
		}
		
		// ce n'est pas une fonction virtuelle !
		void Update(EngineContext& context);

		void PostUpdate();
	};
}
