#pragma once
#include <list>

/*
A room can contain one or none of the wumpus, the gold or a trap. A room adjacent to or containing the wumpus has a stench. A room adjacent
to or containing gold glistens. A room adjactent to or containing a trap is breezy.
*/
namespace RoomNS{
	class Room {
	private:
		int contents;
		int index;

		// Booleans giving information about adjacent rooms.
		bool stenchy;
		bool glistening;
		bool breezy;

		// Pointers to adjacent rooms.
		Room* right;
		Room* left;
		Room* up;
		Room* down;

	public:
		// Codes for the contents of rooms.
		static const int WALL = -1;
		static const int EMPTY = 0;
		static const int EXIT = 1;
		static const int WUMPUS = 2;
		static const int GOLD = 3;
		static const int TRAP = 4;

		static const int STENCHY = 1;
		static const int GLISTENING = 2;
		static const int BREEZY = 3;

		Room();
		Room(int contents);

		void setIndex(int i);
		int getIndex();

		void setStenchy(bool stenchy);
		void setGlistening(bool glistening);
		void setBreezy(bool breezy);

		void setRight(Room* room);
		void setLeft(Room* room);
		void setUp(Room* room);
		void setDown(Room* room);

		Room* getRight();
		Room* getLeft();
		Room* getUp();
		Room* getDown();

		int getContents();
		std::list<int> getSenses();
	};
}
