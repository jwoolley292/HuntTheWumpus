#pragma once
#include "Game.h"
#include <list>
/*
An agent capable of playing Hunt the Wumpus.
*/
namespace AgentNS {
	class Agent
	{
	private:
		/*
		Constants for defining literals. The literal for an individual square will be the condition plus the squares index. For example, the square at
		(1, 1) being breezy has the literal 35. Indicies are given from the top left of the map, going left to right then top to bottom.
		*/
		static const int STENCHY = 1;
		static const int GLISTENING = 17;
		static const int BREEZY = 33;
		static const int EMPTY = 49;
		static const int EXIT = 65;
		static const int WUMPUS = 81;
		static const int GOLD = 97;
		static const int TRAP = 113;

		std::list<std::list<int>> knowledgeBase;

		void initialiseKnowledgeBase();

	public:
		Agent();
	};
}
