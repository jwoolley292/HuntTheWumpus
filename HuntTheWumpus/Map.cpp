#include "Map.h"
#include <time.h>
#include <iostream>

using namespace MapNS;
using namespace RoomNS;
using namespace std;

// Constructor for producing a random map.
Map::Map() {
	rooms = initialiseRandomRooms();
	connectRooms();
	addWalls();
	setRoomStates();
}


// Constructor for producing a specific map.
Map::Map(string seed) {
	rooms = initialiseSetRooms(seed);
	connectRooms();
	addWalls();
	setRoomStates();
}

// Builds a map with a random number of traps. All contents except the exit are randomly distributed.
vector<Room> Map::initialiseRandomRooms() {
	srand((unsigned int)time(NULL));
	vector<Room> r(32);

	r.at(0) = Room(Room::EXIT);
	r.at(1) = Room(Room::WUMPUS);
	r.at(2) = Room(Room::GOLD);

	// Give remaining rooms a 20% chance of containing a trap.
	for (int i = 3; i < 16; i++) {
		if (rand() % 5 == 1) {
			r.at(i) = Room(Room::TRAP);
		}
		else {
			r.at(i) = Room(Room::EMPTY);
		}
	}

	currentRoom = &r.at(0);

	// Swap the Wumpus and Gold with a random location other than (0, 0).
	for (int i = 1; i < 3; i++) {
		int randNum = rand() % 15 + 1;
		Room temp = r.at(randNum);
		r.at(randNum) = r.at(i);
		r.at(i) = temp;
	}

	for (int i = 0; i < 16; i++) {
		r.at(i).setIndex(i);
	}

	return r;
}

/*
Builds a specific map based on the given seed. The seed is a string containing the coordinates of the map contents. The first 4 characters
relate to the wumpus and the gold. Additional characters relate to traps. For example, 11302112 produces a map with the wumpus at (1, 1),
the gold at (3, 0) and traps at (2, 1) and (1, 2).
*/
vector<Room> Map::initialiseSetRooms(string seed) {
	vector<Room> r(32);

	// Start with an empty map.
	r.at(0) = Room(Room::EXIT);
	for (int i = 1; i < 16; i++) {
		r.at(i) = Room(Room::EMPTY);
	}

	currentRoom = &r.at(0);
	
	// Insert the Wumpus.
	int x = seed.at(0) - '0';
	int y = seed.at(1) - '0';
	r.at(x + 4 * y) = Room(Room::WUMPUS);
	seed = seed.substr(2, seed.length() - 2);

	// Insert the gold.
	x = seed.at(0) - '0';
	y = seed.at(1) - '0';
	r.at(x + 4 * y) = Room(Room::GOLD);
	seed = seed.substr(2, seed.length() - 2);

	// Insert any traps.
	while(!seed.empty()) {
		x = seed.at(0) - '0';
		y = seed.at(1) - '0';
		r.at(x + 4 * y) = Room(Room::TRAP);
		seed = seed.substr(2, seed.length() - 2);
		if (seed.empty()) {
			break;
		}
	}

	for (int i = 0; i < 16; i++) {
		r.at(i).setIndex(i);
	}

	return r;
}

// Connects adjacent rooms.
void Map::connectRooms() {
	for (int i = 0; i < 16; i++) {
		if (i % 4 != 3) {
			rooms.at(i).setRight(&rooms.at(i + 1));
		}
		if (i % 4 != 0) {
			rooms.at(i).setLeft(&rooms.at(i - 1));
		}
		if (i > 3) {
			rooms.at(i).setUp(&rooms.at(i - 4));
		}
		if (i < 12) {
			rooms.at(i).setDown(&rooms.at(i + 4));
		}
	}
}

// Adds rooms containing walls to the edge of the map.
void Map::addWalls() {
	int j = 16;
	for (int i = 3; i < 16; i += 4) {
		rooms.at(j) = Room(Room::WALL);
		rooms.at(i).setRight(&rooms.at(j));
		j++;
	}

	for (int i = 0; i < 16; i += 4) {
		rooms.at(j) = Room(Room::WALL);
		rooms.at(i).setLeft(&rooms.at(j));
		j++;
	}

	for (int i = 0; i < 4; i++) {
		rooms.at(j) = Room(Room::WALL);
		rooms.at(i).setUp(&rooms.at(j));
		j++;
	}

	for (int i = 12; i < 16; i++) {
		rooms.at(j) = Room(Room::WALL);
		rooms.at(i).setDown(&rooms.at(j));
		j++;
	}
}

// Stores info about adjacent rooms.
void Map::setRoomStates() {
	for (int i = 0; i < 16; i++) {
		if (rooms.at(i).getContents() == Room::WUMPUS) {
			rooms.at(i).setStenchy(true);
			rooms.at(i).getRight()->setStenchy(true);
			rooms.at(i).getLeft()->setStenchy(true);
			rooms.at(i).getUp()->setStenchy(true);
			rooms.at(i).getDown()->setStenchy(true);
		}

		if (rooms.at(i).getContents() == Room::GOLD) {
			rooms.at(i).setGlistening(true);
			rooms.at(i).getRight()->setGlistening(true);
			rooms.at(i).getLeft()->setGlistening(true);
			rooms.at(i).getUp()->setGlistening(true);
			rooms.at(i).getDown()->setGlistening(true);
		}

		if (rooms.at(i).getContents() == Room::TRAP) {
			rooms.at(i).setBreezy(true);
			rooms.at(i).getRight()->setBreezy(true);
			rooms.at(i).getLeft()->setBreezy(true);
			rooms.at(i).getUp()->setBreezy(true);
			rooms.at(i).getDown()->setBreezy(true);
		}
	}
}

// Returns a string that visually represents the map showing only the exit and player locations.
string Map::drawPlayerMap() {
	string map = "";
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			map += " | ";
			if (&rooms.at(4 * i + j) == currentRoom) {
				map += "P";
			}
			else if (rooms.at(4 * i + j).getContents() == Room::EXIT) {
				map += "E";
			}
			else {
				map += " ";
			}
		}
		map += " | \n";
	}

	return map;
}

// Returns a string that visually represents the contents of the map without the player.
string Map::drawFullMap() {
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

list<int> Map::getSenses() {
	return currentRoom->getSenses();
}

Room* Map::getCurrentRoom() {
	return currentRoom;
}

void Map::setCurrentRoom(Room* room) {
	currentRoom = room;
}

// Returns a seed that can be used to reproduce the map.
string Map::getSeed() {
	string seed = "";

	for (int i = 0; i < 16; i++) {
		if (rooms.at(i).getContents() == Room::WUMPUS) {
			seed += to_string(i % 4) + to_string(i / 4);
			break;
		}
	}

	for (int i = 0; i < 16; i++) {
		if (rooms.at(i).getContents() == Room::GOLD) {
			seed += to_string(i % 4) + to_string(i / 4);
			break;
		}
	}

	for (int i = 0; i < 16; i++) {
		if (rooms.at(i).getContents() == Room::TRAP) {
			seed += to_string(i % 4) + to_string(i / 4);
		}
	}

	return seed;
}
