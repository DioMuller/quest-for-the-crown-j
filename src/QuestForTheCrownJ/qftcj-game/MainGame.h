#pragma once
#include "Game.h"
#include "StructBase.h"
#include "ClientChannel.h"
#include "Scene.h"

namespace qfcgame
{
	class MainGame :
		public qfcbase::Game,
		public qfcbase::IDrawable,
		public qfcbase::IRunnable
	{
		////////////////////////////////////////
		// Attributes
		////////////////////////////////////////
	private:
		std::stack<std::shared_ptr<qfcbase::Scene>> pastScenes;
		bool endScene;
		bool sceneStacked;
	protected:
		std::shared_ptr<qfcbase::Scene> currentScene;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
	public:
		MainGame();
		~MainGame();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
	public:
		virtual void Update(double dt);
		virtual void Draw(sf::RenderWindow* renderer);

		void LoadScene(std::shared_ptr<qfcbase::Scene> s, bool stack = false);
		void UnstackScene();
		bool IsCurrentScene(std::shared_ptr<qfcbase::Scene> scene);

		void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2);
	};
}