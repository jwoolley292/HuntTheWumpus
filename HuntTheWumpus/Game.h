#pragma once
#include "Map.h"
/*
A class containing all of the valid moves a player can make in Hunt the Wumpus as well as variables required to track the game state.
*/
namespace GameNS {
	class Game
	{
	private:
		int score;
		int arrows;
		bool wumpusAlive;
		bool goldAquired;
		MapNS::Map map;

	public:
		// Direction codes.
		static const int RIGHT = 1;
		static const int LEFT = 2;
		static const int UP = 3;
		static const int DOWN = 4;

		// Codes for the results of player actions.
		static const int INVALID_INPUT = -1;
		static const int HIT_WALL = -2;
		static const int DIED = -3;
		static const int EMPTY_ROOM = 0;
		static const int FOUND_GOLD = 1;

		static const int NO_ARROWS = -3;
		static const int MISSED = 1;
		static const int HIT = 2;

		static const int NOT_ON_EXIT = -2;

		// The number of points awarded for different things.
		static const int MOVE_SCORE = -10;
		static const int WUMPUS_SCORE = 1000;
		static const int GOLD_SCORE = 1000;
		static const int DEATH_SCORE = -1000;

		Game();
		Game(std::string mapCode);

		int move(int direction);
		int shoot(int direction);
		int escape();

		int getScore();
		int currentRoomIndex();
		
		std::string drawPlayerMap();
		std::string drawFullMap();
		int getContents();
		std::list<int> getSenses();
		int getArrows();
		bool getGoldAquired();
	};
}
