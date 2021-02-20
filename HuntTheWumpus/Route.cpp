#include "Route.h"

using namespace RouteNS;

Route::Route(int square) {
	currentSquare = square;
	length = 0;
}

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
