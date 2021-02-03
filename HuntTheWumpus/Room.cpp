#include "Room.h"
#include <string>

using namespace RoomNS;
using namespace std;

Room::Room() = default;

Room::Room(int contents) {
	this->contents = contents;

	stench = false;
	glistening = false;
	breezy = false;

	right = NULL;
	left = NULL;
	up = NULL;
	down = NULL;
}

void Room::setStench(bool stench) {
	this->stench = stench;
}

void Room::setGlistening(bool glistening) {
	this->glistening = glistening;
}

void Room::setBreezy(bool breezy) {
	this->breezy = breezy;
}

void Room::setRight(Room room) {
	right = &room;
}

void Room::setLeft(Room room) {
	left = &room;
}

void Room::setUp(Room room) {
	up = &room;
}

void Room::setDown(Room room) {
	down = &room;
}

Room Room::getRight() {
	return *right;
}

Room Room::getLeft() {
	return *left;
}

Room Room::getUp() {
	return *up;
}

Room Room::getDown() {
	return *down;
}

int Room::getContents() {
	return contents;
}

// Method returning a string describing the room
string Room::getSenses() {
	string senses = "";

	if (stench) {
		senses += "Stench";
	}
	if (glistening) {
		if (!senses.empty()) {
			senses += ", ";
		}
		senses += "Glistening";
	}
	if (breezy) {
		if (!senses.empty()) {
			senses += ", ";
		}
		senses += "Breezy";
	}

	if (senses.empty()) {
		senses += "Empty";
	}

	return senses;
}
