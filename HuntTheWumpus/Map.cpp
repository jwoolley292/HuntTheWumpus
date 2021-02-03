#include "Map.h"
#include <time.h>

using namespace MapNS;
using namespace RoomNS;
using namespace std;

// constructor for producing a random map
Map::Map() {
	rooms = initialiseRooms();
	randomiseRooms();
	connectRooms();
	addWalls();
	setRoomStates();
}

/*
Constructor for producing a specific map. The map code is an 8 digit string containing the coordinates of the map contents. For example,
11302112 produces a map with the wumpus at (1, 1), the gold at (3, 0) and traps at (2, 1) and (1, 2).
*/
Map::Map(string mapCode) {
	rooms = initialiseRooms();
	connectRooms();
	addWalls();
	setRoomStates();
}

vector<RoomNS::Room> Map::initialiseRooms() {
	vector<Room> r(32);
	
	currentRoom = Room(Room::EXIT);
	r.at(0) = currentRoom;

	r.at(1) = Room(Room::WUMPUS);
	r.at(2) = Room(Room::GOLD);
	r.at(3) = Room(Room::TRAP);
	r.at(4) = Room(Room::TRAP);

	for (int i = 5; i < 16; i++) {
		r.at(i) = Room(Room::EMPTY);
	}

	return r;
}

void Map::randomiseRooms() {
	srand((unsigned int)time(NULL));
	for (int i = 1; i < 5; i++) {
		int randNum = rand() % 15 + 1;
		Room temp = rooms.at(randNum);
		rooms.at(randNum) = rooms.at(i);
		rooms.at(i) = temp;
	}
}

void Map::connectRooms() {
	for (int i = 0; i < 16; i++) {
		if (i % 4 != 3) {
			rooms.at(i).setRight(rooms.at(i + 1));
		}
		if (i % 4 != 0) {
			rooms.at(i).setLeft(rooms.at(i - 1));
		}
		if (i < 12) {
			rooms.at(i).setUp(rooms.at(i + 4));
		}
		if (i > 3) {
			rooms.at(i).setDown(rooms.at(i - 4));
		}
	}
}

// Adds rooms containing walls to the edge of the map
void Map::addWalls() {
	for (int i = 3; i < 16; i += 4) {
		rooms.at(i).setRight(Room(Room::WALL));
	}

	for (int i = 0; i < 16; i += 4) {
		rooms.at(i).setLeft(Room(Room::WALL));
	}

	for (int i = 12; i < 16; i++) {
		rooms.at(i).setUp(Room(Room::WALL));
	}

	for (int i = 0; i < 4; i++) {
		rooms.at(i).setDown(Room(Room::WALL));
	}
}

// Stores info about adjacent rooms in the main room objects
void Map::setRoomStates() {
	for (int i = 0; i < 16; i++) {
		if (rooms.at(i).getContents() == Room::WUMPUS) {
			rooms.at(i).getRight().setStench(true);
			rooms.at(i).getLeft().setStench(true);
			rooms.at(i).getUp().setStench(true);
			rooms.at(i).getDown().setStench(true);
		}

		if (rooms.at(i).getContents() == Room::GOLD) {
			rooms.at(i).getRight().setGlistening(true);
			rooms.at(i).getLeft().setGlistening(true);
			rooms.at(i).getUp().setGlistening(true);
			rooms.at(i).getDown().setGlistening(true);
		}

		if (rooms.at(i).getContents() == Room::TRAP) {
			rooms.at(i).getRight().setBreezy(true);
			rooms.at(i).getLeft().setBreezy(true);
			rooms.at(i).getUp().setBreezy(true);
			rooms.at(i).getDown().setBreezy(true);
		}
	}
}

// returns a string that visually represents the contents of the map without the player
string Map::drawMap() {
	string map = "";

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			string contents;
			switch (rooms.at(4 * i + j).getContents()) {
			case Room::EMPTY:
				contents = " ";
				break;
			case Room::EXIT:
				contents = "E";
				break;
			case Room::WUMPUS:
				contents = "W";
				break;
			case Room::GOLD:
				contents = "G";
				break;
			case Room::TRAP:
				contents = "T";
				break;
			}

			map += " | " + contents;
		}

		map += " | \n";
	}

	return map;
}