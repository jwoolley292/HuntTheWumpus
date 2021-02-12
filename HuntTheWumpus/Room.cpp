#include "Room.h"
#include <string>

using namespace RoomNS;
using namespace std;

Room::Room() {
	contents = EMPTY;
	index = -1;

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
	index = -1;

	stenchy = false;
	glistening = false;
	breezy = false;

	right = NULL;
	left = NULL;
	up = NULL;
	down = NULL;
}

void Room::setIndex(int i) {
	index = i;
}

int Room::getIndex() {
	return index;
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

// Method returning a string describing the room.
list<int> Room::getSenses() {
	list<int> senses;

	if (stenchy) {
		senses.push_back(STENCHY);
	}
	if (glistening) {
		senses.push_back(GLISTENING);
	}
	if (breezy) {
		senses.push_back(BREEZY);
	}

	return senses;
}
