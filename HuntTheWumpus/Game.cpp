#include "Game.h"
#include <iostream>

using namespace GameNS;
using namespace MapNS;
using namespace RoomNS;
using namespace std;

// Constructor for a random map.
Game::Game() {
	map = Map();
	wumpusAlive = true;
	goldAquired = false;
	arrows = 1;
	score = 0;
}

// Constructor for a map specified by a seed.
Game::Game(string seed) {
	map = Map(seed);
	wumpusAlive = true;
	goldAquired = false;
	arrows = 1;
	score = 0;
}

// Moves the player one square in the chosen direction unless the target square is a wall.
int Game::move(int direction) {
	if (direction < 1 || direction > 4) {
		throw "Invalid input: direction must be between 1 and 4";
	}

	Room* target;
	switch (direction) {
	case RIGHT:
		target = map.getCurrentRoom()->getRight();
		break;
	case LEFT:
		target = map.getCurrentRoom()->getLeft();
		break;
	case UP:
		target = map.getCurrentRoom()->getUp();
		break;
	case DOWN:
		target = map.getCurrentRoom()->getDown();
		break;
	default:
		return INVALID_INPUT;
		break;
	}

	if (target->getContents() == Room::WALL) {
		return HIT_WALL;
	}
	else {
		map.setCurrentRoom(target);
		score += MOVE_SCORE;
	}
	if (target->getContents() == Room::TRAP || (target->getContents() == Room::WUMPUS && wumpusAlive)) {
		score += DEATH_SCORE;
		return DIED;
	}
	else if (target->getContents() == Room::GOLD && !goldAquired) {
		goldAquired = true;
		return FOUND_GOLD;
	}
	return EMPTY_ROOM;
}


// Shoots the adjacent square unless that square is a wall. If a shot is fired, this consumes an arrow. If it hits the wumpus, the wumpus dies.
int Game::shoot(int direction) {
	if (direction < 1 || direction > 4) {
		throw "Invalid input: direction must be between 1 and 4";
	}

	if (arrows < 1) {
		return NO_ARROWS;
	}

	Room* target;
	switch (direction) {
	case RIGHT:
		target = map.getCurrentRoom()->getRight();
		break;
	case LEFT:
		target = map.getCurrentRoom()->getLeft();
		break;
	case UP:
		target = map.getCurrentRoom()->getUp();
		break;
	case DOWN:
		target = map.getCurrentRoom()->getDown();
		break;
	default:
		return INVALID_INPUT;
	}

	if (target->getContents() == Room::WALL) {
		return HIT_WALL;
	}
	if (target->getContents() == Room::WUMPUS) {
		wumpusAlive = false;
		arrows--;
		return HIT;
	}
	arrows--;
	return MISSED;
}

// If the player is on the exit, this calculates and returns their final score.
int Game::escape() {
	if (map.getCurrentRoom()->getContents() != Room::EXIT) {
		return NOT_ON_EXIT;
	}
	if (!wumpusAlive) {
		score += WUMPUS_SCORE;
	}
	if (goldAquired) {
		score += GOLD_SCORE;
	}
	return score;
}

int Game::getScore() {
	return score;
}

int Game::currentRoomIndex() {
	return map.getCurrentRoom()->getIndex();
}

string Game::drawPlayerMap() {
	return map.drawPlayerMap();
}

string Game::drawFullMap() {
	return map.drawFullMap();
}

int Game::getContents() {
	return map.getCurrentRoom()->getContents();
}

list<int> Game::getSenses() {
	return map.getSenses();
}

int Game::getArrows() {
	return arrows;
}

bool Game::getGoldAquired() {
	return goldAquired;
}

string Game::getSeed() {
	return map.getSeed();
}
