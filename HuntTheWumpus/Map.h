#pragma once
#include "Room.h"
#include <string>
#include <vector>

/*
A map is a 4 x 4 grid of rooms. The room at 0, 0 is the start point and exit and is always empty and at the top left of the map. 1 wumpus, 1 gold
and 2 traps are distributed amongst the remaining rooms.
*/
namespace MapNS {
	class Map {
	private:
		std::vector<RoomNS::Room> rooms;
		RoomNS::Room currentRoom;

		std::vector<RoomNS::Room> initialiseRooms();
		void randomiseRooms();
		void connectRooms();
		void addWalls();
		void setRoomStates();
	public:
		Map();
		Map(std::string mapCode);

		std::string drawMap();
	};
}
