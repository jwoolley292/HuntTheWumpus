#pragma once
#include "Game.h"
#include "Agent.h"
/*
A console based application for playing Hunt the Wumpus or creating an agent to play.
*/
namespace ApplicationNS {
	class Application
	{
	private:
		GameNS::Game game;
		AgentNS::Agent agent;
		void gameMenu();
		std::string getSeed();

		void move(int direction);
		void shoot();
		void escape();
		void endGame();

		int agentPlay();
		std::list<int> generateMoveUpdate();

		void printSenses();
		std::string setStringToLowerCase(std::string toConvert);

	public:
		Application();
		void mainMenu();
	};
}
