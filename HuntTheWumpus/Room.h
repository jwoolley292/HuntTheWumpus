#pragma once
#include <string>

/*
A room can contain one or none of the wumpus, the gold or a trap. A room adjacent to or containing the wumpus has a stench. A room adjacent
to or containing gold glistens. A room adjactent to or containing a trap is breezy.
*/
namespace RoomNS{
	class Room {
	private:
		int contents;

		// booleans giving information about adjacent rooms
		bool stench;
		bool glistening;
		bool breezy;

		Room* right;
		Room* left;
		Room* up;
		Room* down;

	public:
		// constants referring to the contents of the room
		static const int WALL = -1;
		static const int EMPTY = 0;
		static const int EXIT = 1;
		static const int WUMPUS = 2;
		static const int GOLD = 3;
		static const int TRAP = 4;

		Room();
		Room(int contents);

		void setStench(bool stench);
		void setGlistening(bool glistening);
		void setBreezy(bool breezy);

		void setRight(Room room);
		void setLeft(Room room);
		void setUp(Room room);
		void setDown(Room room);

		Room getRight();
		Room getLeft();
		Room getUp();
		Room getDown();

		int getContents();
		std::string getSenses();
	};
}

