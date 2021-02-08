#include "Room.h"
#include <string>

using namespace RoomNS;
using namespace std;

Room::Room() {
	contents = EMPTY;

	stenchy = false;
	glistening = false;
	breezy = false;

	right = NULL;
	left = NULL;
	up = NULL;
	down = NULL;
}

Room::Room(int contents) {
	this->contents = contents;

	stenchy = false;
	glistening = false;
	breezy = false;

	right = NULL;
	left = NULL;
	up = NULL;
	down = NULL;
}

void Room::setStenchy(bool stenchy) {
	this->stenchy = stenchy;
}

void Room::setGlistening(bool glistening) {
	this->glistening = glistening;
}

void Room::setBreezy(bool breezy) {
	this->breezy = breezy;
}

void Room::setRight(Room* room) {
	right = room;
}

void Room::setLeft(Room* room) {
	left = room;
}

void Room::setUp(Room* room) {
	up = room;
}

void Room::setDown(Room* room) {
	down = room;
}

Room* Room::getRight() {
	return right;
}

Room* Room::getLeft() {
	return left;
}

Room* Room::getUp() {
	return up;
}

Room* Room::getDown() {
	return down;
}

int Room::getContents() {
	return contents;
}

// Removes gold from the room and removes the glistening status from it and adjacent rooms.
void Room::removeGold() {
	if (contents == GOLD) {
		contents = EMPTY;
		setGlistening(false);
		right->setGlistening(false);
		left->setGlistening(false);
		up->setGlistening(false);
		down->setGlistening(false);
	}
}

// Method returning a string describing the room.
string Room::getSenses() {
	string senses = "";

	if (stenchy) {
		senses += "Stenchy";
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
