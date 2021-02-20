#pragma once
#include <list>
/*
A class for holding a square reached when plotting a route, the first action taken and the number of actions taken so far.
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
