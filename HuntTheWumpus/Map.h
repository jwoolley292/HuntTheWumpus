#pragma once
#include "Room.h"
#include <string>
#include <vector>

/*
A map is a 4 x 4 grid of rooms. The room at (0, 0) is the start point and exit and at the top left of the map. 1 wumpus, 1 gold and any
number of traps are distributed amongst the remaining rooms.
*/
namespace MapNS {
	class Map {
	private:
		std::vector<RoomNS::Room> rooms;
		RoomNS::Room* currentRoom;

		std::vector<RoomNS::Room> initialiseRandomRooms();
		std::vector<RoomNS::Room> initialiseSetRooms(std::string mapCode);
		void connectRooms();
		void addWalls();
		void setRoomStates();

	public:
		Map();
		Map(std::string seed);

		std::string drawPlayerMap();
		std::string drawFullMap();
		std::list<int> getSenses();
		RoomNS::Room* getCurrentRoom();
		void setCurrentRoom(RoomNS::Room* room);
		std::string getSeed();
	};
}
