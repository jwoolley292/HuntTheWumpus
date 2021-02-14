#include "Application.h"
#include <iostream>

using namespace ApplicationNS;
using namespace GameNS;
using namespace AgentNS;
using namespace RoomNS;
using namespace std;

Application::Application() {
	cout << "Welcome to Hunt the Wumpus\n\n";
}

// The main menu in which the player chooses what they want to do.
void Application::mainMenu() {
	cout << "1: Play on random map\n";
	cout << "2: Play on set map\n";
	cout << "3: Agent on random map\n";
	cout << "4: Agent on set map\n";
	cout << "5: Agent on multiple maps\n";
	cout << "6: Exit\n";
	cout << "\n";

	string command = "";
	while (command.empty()) {
		cin >> command;
		command = setStringToLowerCase(command);
		cout << "\n";

		if (command.compare("1") == 0) {
			game = Game();
			gameMenu();
		}
		else if (command.compare("2") == 0) {
			game = Game(getSeed());
			cout << game.drawFullMap() << "\n\n";
			gameMenu();
		}
		else if (command.compare("3") == 0) {
			game = Game();
			agent = Agent();
			cout << game.drawFullMap() << "\n\n";
			agentPlay();
			mainMenu();
		}
		else if (command.compare("4") == 0) {
			game = Game(getSeed());
			agent = Agent();
			cout << game.drawFullMap() << "\n\n";
			agentPlay();
			mainMenu();
		}
		else if (command.compare("5") == 0) {

		}
		else if (command.compare("6") == 0) {
			exit(0);
		}
		else {
			command = "";
			cout << "Please enter a number between 1 and 6\n\n";
		}
	}
}
/*
Gets and validates a seed for generating a map. The seed is an string containing the coordinates of the map contents. The first 4 characters relate to the
wumpus and the gold. Additional characters relate to traps. For example, 11302112 produces a map with the wumpus at(1, 1), the gold at(3, 0) and traps at
(2, 1) and (1, 2).
*/
string Application::getSeed() {
	cout << "Input seed\n\n";

	string seed = "";
	while (seed.empty()) {
		cin >> seed;
		cout << "\n";

		bool allCharactersValid = seed.find_first_not_of("0123") == string::npos;
		bool correctLength = seed.length() <= 16 && seed.length() >= 4;
		bool evenNumOfChars = seed.length() % 2 == 0;

		bool nothingAt00 = true;
		if (allCharactersValid && correctLength && evenNumOfChars) {
			for (int i = 0; i < seed.length(); i += 2) {
				if (seed.substr(i, 2).compare("00") == 0) {
					nothingAt00 = false;
					break;
				}
			}
		}

		bool noRepititions = true;
		if (allCharactersValid && correctLength && evenNumOfChars && nothingAt00) {
			for (int i = 0; i < seed.length(); i += 2) {
				string sub1 = seed.substr(i, 2);
				for (int j = i + 2; j < seed.length(); j += 2) {
					string sub2 = seed.substr(j, 2);
					if (sub1.compare(sub2) == 0) {
						noRepititions = false;
						break;
					}
				}
			}
		}

		if (!allCharactersValid || !correctLength || !evenNumOfChars || !nothingAt00 || !noRepititions) {
			cout << "Invalid input. The seed is an string containing the coordinates of the map contents. The first 4 characters relate to the ";
			cout << "wumpus and the gold.Additional characters relate to traps.For example, 11302112 produces a map with the wumpus at(1, 1), the ";
			cout << "gold at(3, 0) and traps at (2, 1) and (1, 2).\n\n";
			seed = "";
		}
	}
	return seed;
}

// Draws the map showing the player and exit locations, then takes the players input.
void Application::gameMenu() {
	cout << game.drawPlayerMap() << "\n\n";
	printSenses();

	string command = "";
	while (command.empty()) {
		cin >> command;
		command = setStringToLowerCase(command);
		cout << "\n";

		if (command.compare("d") == 0) {
			move(Game::RIGHT);
		}
		else if (command.compare("a") == 0) {
			move(Game::LEFT);
		}
		else if (command.compare("w") == 0) {
			move(Game::UP);
		}
		else if (command.compare("s") == 0) {
			move(Game::DOWN);
		}
		else if (command.compare("z") == 0) {
			shoot();
		}
		else if (command.compare("x") == 0) {
			escape();
		}
		else if (command.compare("exit") == 0) {
			endGame();
		}
		else {
			command = "";
			cout << "Invald input.\n\n";
			cout << "w: move up\n";
			cout << "a: move left\n";
			cout << "s: move down\n";
			cout << "d: move right\n";
			cout << "z: shoot\n";
			cout << "x: escape\n";
			cout << "exit: end game\n\n";
		}
	}
}
/*
Moves in the chosen direction unless there is a wall there. If the player dies, the full map is printed along with the players final score. If they find
gold, they are notified.
*/
void Application::move(int direction) {
	if (direction < 1 || direction > 4) {
		throw "Invalid input: direction must be between 1 and 4";
	}
	
	int result = Game::INVALID_INPUT;
	switch (direction) {
	case Game::RIGHT:
		result = game.move(Game::RIGHT);
		break;
	case Game::LEFT:
		result = game.move(Game::LEFT);
		break;
	case Game::UP:
		result = game.move(Game::UP);
		break;
	case Game::DOWN:
		result = game.move(Game::DOWN);
		break;
	}

	if (result == Game::HIT_WALL) {
		cout << "You hit a wall\n\n";
		gameMenu();
	}
	else if (result == Game::DIED) {
		cout << game.drawFullMap() << "\n\n";
		cout << "You died! Your final score is: " << game.getScore() << "\n\n";
		mainMenu();
	}
	else if (result == Game::EMPTY_ROOM) {
		gameMenu();
	}
	else if (result == Game::FOUND_GOLD) {
		if (!game.getGoldAquired()) {
			cout << "You found gold!\n\n";
		}
		gameMenu();
	}
}

// Checks the direction the player wants to shoot in, shoots if they aren't shooting at a wall and updates the player.
void Application::shoot() {
	if (game.getArrows() < 1) {
		cout << "You have no arrows left\n\n";
		gameMenu();
	}
	else {
		cout << "Which direction would you like to shoot in?\n";

		int result = Game::INVALID_INPUT;
		string command = "";
		while (command.empty()) {
			cin >> command;
			command = setStringToLowerCase(command);
			cout << "\n";

			if (command.compare("d") == 0) {
				result = game.shoot(Game::RIGHT);
			}
			else if (command.compare("a") == 0) {
				result = game.shoot(Game::LEFT);
			}
			else if (command.compare("w") == 0) {
				result = game.shoot(Game::UP);
			}
			else if (command.compare("s") == 0) {
				result = game.shoot(Game::DOWN);
			}
			else if (command.compare("x") == 0) {
				gameMenu();
			}
			else {
				command = "";
				cout << "Invald input.\n\n";
				cout << "w: shoot up\n";
				cout << "a: shoot left\n";
				cout << "s: shoot down\n";
				cout << "d: shoot right\n";
				cout << "x: cancel\n\n";
			}
		}

		if (result == Game::MISSED) {
			cout << "Your arrow disappears silently into the darkness\n\n";
			gameMenu();
		}
		else if (result == Game::HIT) {
			cout << "You hear an agonised cry from the darkness\n\n";
			gameMenu();
		}
		else if (result == Game::HIT_WALL) {
			cout << "There's a wall there\n\n";
			gameMenu();
		}
	}
}

// Ends the game if the player is on the exit, printing their final score and the full map.
void Application::escape() {
	int result = game.escape();
	if (result == Game::NOT_ON_EXIT) {
		cout << "Head to the exit first\n\n";
		gameMenu();
	}
	else {
		cout << game.drawFullMap() << "\n\n";
		cout << "You escaped! Your final score is: " << result << "\n\n";
		mainMenu();
	}
}

// Ends the game early after confirming this is the player's intent.
void Application::endGame() {
	cout << "Are you sure you want to exit? Y/N\n\n";

	string command = "";
	while (command.empty()) {
		cin >> command;
		setStringToLowerCase(command);
		cout << "\n";
	
		if(command.compare("y") == 0) {
			mainMenu();
		}
		else if (command.compare("n") == 0) {
			gameMenu();
		}
		else {
			command = "";
			cout << "Enter y for yes or n for no\n\n";
		}
	}
}

int Application::agentPlay() {
	agent.updateKnowledgeBase(generateMoveUpdate());
	while (true) {
		int action = agent.getAction();
		int result;
		if (action <= Agent::MOVE_DOWN) {
			result = game.move(action);
			if (result == Game::DIED) {
				cout << game.drawFullMap() << "\n\n";
				cout << "You died! Your final score is: " << game.getScore() << "\n\n";
				return game.getScore();
			}
			else if (result == Game::EMPTY_ROOM) {
				agent.updateKnowledgeBase(generateMoveUpdate());
			}
			else if (result == Game::FOUND_GOLD) {
				if (!game.getGoldAquired()) {
					cout << "You found gold!\n\n";
				}
				agent.updateKnowledgeBase(list<int> { Agent::GOLD_FOUND });
				agent.updateKnowledgeBase(generateMoveUpdate());
			}
		}
		else if (action <= Agent::SHOOT_DOWN) {
			result = game.shoot(action - 4);
			if (result == Game::MISSED) {
				cout << "Your arrow disappears silently into the darkness\n\n";
				agent.updateKnowledgeBase({ -Agent::SHOT_HIT });
			}
			else if (result == Game::HIT) {
				cout << "You hear an agonised cry from the darkness\n\n";
				agent.updateKnowledgeBase({ Agent::SHOT_HIT });
			}
		}
		else {
			result = game.escape();
			cout << "You escaped! Your final score is: " << result << "\n\n";
			return result;
		}
	}
}

/*
Generates a list of literals with which to update the agent after moving.
*/
list<int> Application::generateMoveUpdate() {
	list<int> update;

	int contents = game.getContents();
	if (contents == Room::EMPTY) {
		update.push_back(Agent::EMPTY + game.currentRoomIndex());
	}
	else if (contents == Room::EXIT) {
		update.push_back(Agent::EXIT + game.currentRoomIndex());
	}
	else if (contents == Room::WUMPUS) {
		update.push_back(Agent::WUMPUS + game.currentRoomIndex());
	}
	else if (contents == Room::GOLD) {
		update.push_back(Agent::GOLD + game.currentRoomIndex());
	}
	else if (contents == Room::TRAP) {
		update.push_back(Agent::TRAP + game.currentRoomIndex());
	}

	list<int> senses = game.getSenses();
	if (find(senses.begin(), senses.end(), Room::STENCHY) != senses.end()) {
		update.push_back(Agent::STENCHY + game.currentRoomIndex());
	}
	else {
		update.push_back(-(Agent::STENCHY + game.currentRoomIndex()));
	}

	if (find(senses.begin(), senses.end(), Room::GLISTENING) != senses.end()) {
		update.push_back(Agent::GLISTENING + game.currentRoomIndex());
	}
	else {
		update.push_back(-(Agent::GLISTENING + game.currentRoomIndex()));
	}

	if (find(senses.begin(), senses.end(), Room::BREEZY) != senses.end()) {
		update.push_back(Agent::BREEZY + game.currentRoomIndex());
	}
	else {
		update.push_back(-(Agent::BREEZY + game.currentRoomIndex()));
	}

	for (list<int>::iterator i = update.begin(); i != update.end(); i++) {
		cout << *i << " ";
	}
	cout << "\n\n";

	return update;
}

void Application::printSenses() {
	list<int> senses = game.getSenses();
	string sensesString = "The room is: ";
	if (senses.empty()) {
		sensesString += "empty ";
	}
	else {
		for (list<int>::iterator i = senses.begin(); i != senses.end(); i++) {
			if (*i == Room::STENCHY) {
				sensesString += "stenchy ";
			}
			else if (*i == Room::GLISTENING) {
				sensesString += "glistening ";
			}
			else if (*i == Room::BREEZY) {
				sensesString += "breezy ";
			}
		}
	}
	cout << sensesString << "\n\n";
}

string Application::setStringToLowerCase(string toConvert) {
	for (int i = 0; i < toConvert.length(); i++) {
		toConvert.at(i) = tolower(toConvert.at(i));
	}
	return toConvert;
}

int main() {
	Application app = Application();
	app.mainMenu();
}
