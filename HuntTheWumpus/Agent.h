#pragma once
#include "Game.h"
#include "Route.h"
#include <list>
/*
An agent capable of playing Hunt the Wumpus.
*/
namespace AgentNS {
	class Agent
	{
	private:
		std::list<std::list<int>> knownClauses;
		std::list<int> unknownLiterals;
		std::list<int> knownLiterals;
		int currentLocation;
		std::list<int> visited;
		std::list<int> visitedAdjacent;

		void initialiseKnowledgeBase();
		void setGuaranteedContents();
		void squareContainsOneThing(int square);
		void setUniqueContents(int square);
		void associateContentsWithSenses(int square);
		void associateSensesWithContents(int square);

		std::list<std::list<int>> simplifyClauses(std::list<std::list<int>> clauses, int l);
		void updateVisited(int location);
		void prioritiseUnknownLiterals(std::list<int> literals);
		std::list<int> locationPriorities();
		void makeInferences(std::list<int> literals);

		bool DPLL(std::list<std::list<int>> clauses, std::list<int> literals, std::list<int> partialModel);
		int checkModel(std::list<std::list<int>> clauses, std::list<int> partialModel);
		int checkClause(std::list<int> clause, std::list<int> partialModel);
		int findPureLiteral(std::list<std::list<int>> clauses, std::list<int> literals);
		int findUnitClause(std::list<std::list<int>> clauses);

		int escape();
		int collectGold(int location);
		int shootWumpus(int location);
		int findGold();
		int findWumpus();
		int explore();

		int pathFind(std::list<int> goals);
		int estimateRouteDistance(RouteNS::Route route, std::list<int> goals);
		int safeSquare(int square);

		void printKnowledgeBase(std::list<std::list<int>> clauses, std::list<int> literals, std::list<int> partialModel);
	public:
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
		static const int SHOT_HIT = 129;
		static const int GOLD_AQUIRED = 130;

		static const int TRUE_INT = 1;
		static const int FALSE_INT = 0;
		static const int NULL_INT = -1;

		// Constants representing the actions available to the agent.
		static const int MOVE_RIGHT = 1;
		static const int MOVE_LEFT = 2;
		static const int MOVE_UP = 3;
		static const int MOVE_DOWN = 4;
		static const int SHOOT_RIGHT = 5;
		static const int SHOOT_LEFT = 6;
		static const int SHOOT_UP = 7;
		static const int SHOOT_DOWN = 8;
		static const int ESCAPE = 9;

		Agent();
		void updateKnowledgeBase(int location, std::list<int> literals);
		int getAction();
		void printKnownLiterals();

		// Methods for testing.
		void addClause(std::list<int> clause);
		void addUnknownLiterals(std::list<int> literals);
		std::list<int> getKnownLiterals();
	};
}
