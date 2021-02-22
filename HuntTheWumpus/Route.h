#pragma once
#include <list>
/*
A class for holding a list of actions required to reach a given square.
*/
namespace RouteNS {
	class Route
	{
	private:
		std::list<int> actions;
		int currentSquare;
		int length;
	public:
		Route(int square);
		void update(int action, int nextSquare);
		int getCurrentSquare();
		int getFirstAction();
		int getLength();
	};
}
