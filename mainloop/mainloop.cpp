//
// ESGI - Cours Developpement Moteur C++
//
// Exemple de boucle principale 
//
// Appuyez sur Echap (ESC) pour quitter la boucle
//


#include "Utils.h"
#include "Timer.h"
#include "Input.h"
#include "Engine.h"
#include "Renderer.h"
#include "EngineContext.h"


#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include "JobSystem.h"


namespace ESGI
{
	template <typename T>
	T clamp(T val, T min, T max)
	{
		return val > max ? max : val < min ? min : val;
	}
	
	//
	// fonctions globales
	//

	void OneTimeEvent() {
		std::cout << "One Time Event\n";
	}

	void RecurringTimeEvent() {
		std::cout << "Recurring Time Event\n";
	}

	// Je nomme volontairement ces fonctions du meme nom que les classes pour souligner un point important.
	// En C++ moderne, le compilateur reconnait qu'il s'agit la d'un appel de fonction
	// et pas d'un appel au constructeur car il interprete les "()" comme un appel de fonction.
	// Et comme il existe bien des fonctions ayant ce nom et les parentheses, il arrive a resoudre le lien.
	// Par contre si les lignes suivantes sont commentees, le compilateur n'aura d'autre choix
	// que d'appeler le constructeur, seule fonction qui ressemble a cet appel - 
	// ce qui engendre un comportement menant au crash dans notre cas
	Clock& EngineContext::Clock() const { return clock; }
	Input& EngineContext::Input() const { return input; }
	Engine& EngineContext::Engine() const { return engine; }
	Renderer& EngineContext::Renderer() const { return renderer; }
	// Renderer& EngineContext::Engine() const { return renderer; }

	// une map ou une recherche par identifiant dans un vector serait sans doute plus elegant et generique

	//
	// Application
	//

	struct Application
	{
		// ce vecteur va nous permettre d'iterer sur l'ensemble des elements du moteur sans les nommer individuellement
		std::vector<InfraStructure*> m_cores;

		// les references sont plus complexes a initialiser (via le constructeur seulement)
		EngineContext& m_context;
		
		uint64_t m_frameIndex = 0;

		bool m_needToQuit = false;

		Application(EngineContext& context) : m_context(context) {}
		Application() :
			Application(Application::CreateContext())				// Depuis le C++11 un constructeur peut en appeler un autre
		{}
		Application(Application& app) = delete;						// une application ne peut etre dupliquee



		// Le fait de creer explicitement les elements important du moteur nous permet de mieux
		// maitriser la gestion memoire mais plus particulierement l'ordre de construction/destruction
		// chose que l'on ne pourrait pas faire avec des singletons
		// De plus, en regroupant toutes ces fonctions dans une seule classe (EngineContext ici) contenant des pointeurs
		// on simplifie l'interface en ne passant que la structure lorsque necessaire
		//
		// la fonction est tout de meme dangereuse car rien n'empeche un utilisateur de l'appeler plusieurs fois
		// elle devrait etre idealement privee mais c'est ok pour nous
		//
		// notez l'usage d'un pseudo-singleton sous la forme d'une seule variable statique, ce qui simplifie enormement la gestion		
		//
		static EngineContext& CreateContext()
		{
			Clock* clock = new ESGI::Clock;
			Input* input = new ESGI::Input;
			Engine* engine = new ESGI::Engine;
			Renderer* renderer = new ESGI::Renderer;

			static EngineContext context(*clock, *input, *engine, *renderer);
			return context;
		}

		// de plus, comme CreateContext() fait partie de Application et pas de EngineContext c'est donc a Application
		// qu'il revient de gerer les classes Clock, Input et Engine ...

		bool Create()
		{
			// l'ordre peut etre important si c'est l'ordre d'update
			m_cores.reserve(3);
			m_cores.emplace_back(&m_context.clock);
			m_cores.emplace_back(&m_context.input);
			m_cores.emplace_back(&m_context.engine);
			m_cores.emplace_back(&m_context.renderer);

			return true;
		}

		//  ... et c'est donc la classe Application qui doit les desallouer dans Destroy()

		void Destroy()
		{
			for (auto* core : make_reverse(m_cores)) {
#if defined(_DEBUG)
				std::cout << "destroying core " << core->DebugName().c_str() << std::endl;
#endif
				delete core;
			}

			m_cores.clear();			// resize a zero element (mais la capacite ne change pas)
			m_cores.shrink_to_fit();	// ajuste la capacity a la size du vector (libere la memoire)
		}

		bool Initialize()
		{
			JobSystem::Initialize();
			bool allOk = true;
			for (auto* core : m_cores) {
				allOk &= core->Initialize();
			}
			Engine& eng = m_context.Engine();

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937 generator(seed);
			std::uniform_real_distribution<double> uniform01(0.0, 1.0);
			
			for (size_t i = 0; i < 200; i++)
			{
				auto ent = eng.m_AIEngine->world->CreateEntity();
				
				eng.m_AIEngine->world->AddComponent<ECS::RendererComponent>(ent,
					{ m_context.Renderer().cubeVAO,
					m_context.Renderer().defaultShaderProgram });
				
				eng.m_AIEngine->world->AddComponent<ECS::TransformComponent>(ent,
					{ });

				eng.m_AIEngine->world->AddComponent<ECS::AgentFlockComponent>(ent,
					{ });
				
				eng.m_AIEngine->world->AddComponent<ECS::LifeTimeComponent>(ent,
					{ (float)uniform01(generator) * 10.0f + 0.5f, (float)uniform01(generator) * 5.0f + 0.5f });
			}

			// exemple de scheduling de deux fonctions (non membre, plus simple a faire)
			// todo: event/delegate facon c# acceptant tout type de fonction.

			ESGI::Timer timer1{ 9.0, 0.0, false };
			m_context.Clock().AddTimer(timer1, &OneTimeEvent);

			ESGI::Timer timer2{ 5.0, 0.0, true };
			m_context.Clock().AddTimer(timer2, &RecurringTimeEvent);

			return allOk;
		}

		void DeInitialize()
		{
			JobSystem::Wait();
			
			for (auto* core : make_reverse(m_cores)) {
				core->DeInitialize();
			}
		}

		void Update()
		{
			// Ici le role et l'ordre de chaque classe est bien defini 
			m_context.Renderer().PreUpdate();

			m_context.Clock().Update();

			m_context.Input().Update();

			m_context.Engine().Update(m_context);

			double elapsedTime = m_context.clock.ElapsedTime;

			// tout lag est maximise 100 ms (1/10 de seconde)
			// cela permet d'eviter de faire sauter le moteur en cas de breakpoint ou lag enorme
			// alternativement on peut appliquer une correction en extrapolant a partir du temps ecoule 
			if (elapsedTime > 0.10)
				elapsedTime = 0.10;
			
			m_context.Renderer().Update(elapsedTime);

			m_context.Renderer().PostUpdate();

			m_context.Engine().PostUpdate();
		}

		// main loop
		void Run()
		{
			bool initOk = Create();

			if (initOk) {
				initOk &= Initialize();
			}

			m_needToQuit = !initOk;

			m_context.Engine().Start();
			
			while (!m_needToQuit && !m_context.Renderer().WindowShouldClose())
			{
				//std::cout << "[Application] frame # " << m_frameIndex << std::endl;

				Update();

				// emule un delai de traitement (une synchro verticale par ex.)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

				m_needToQuit = m_context.Input().QuitButtonPressed;

				//m_frameIndex++;
			}

			DeInitialize();

			Destroy();
		}
	};
}

#include <Windows.h>

int main(void)
{
	using namespace ESGI;

	Application gameEngine;

	gameEngine.Run();


	return 0;
}