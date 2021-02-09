#pragma once
#include "Game.h"
/*
A console based application for playing Hunt the Wumpus or creating an agent to play.
*/
namespace ApplicationNS {
	class Application
	{
	private:
		GameNS::Game game;
		void gameMenu();
		void getSeed();

		void move(int direction);
		void shoot();
		void escape();
		void endGame();
		std::string setStringToLowerCase(std::string toConvert);

	public:
		Application();
		void mainMenu();
	};
}
