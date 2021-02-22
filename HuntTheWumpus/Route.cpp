#include "Route.h"

using namespace RouteNS;

Route::Route(int square) {
	currentSquare = square;
	length = 0;
}

/*
Takes in a new action and the square reached via that action and stores them.
*/
void Route::update(int action, int square) {
	currentSquare = square;
	actions.push_back(action);
	length++;
}

int Route::getCurrentSquare() {
	return currentSquare;
}

int Route::getFirstAction() {
	return actions.front();
}

int Route::getLength() {
	return length;
}
