#include "Application.h"
#include <iostream>

using namespace ApplicationNS;
using namespace MapNS;
using namespace std;

Application::Application() {
	cout << "Welcome to Hunt the Wumpus\n\n";
}

void Application::mainMenu() {

}

void main() {
	Application app = Application();
	cout << Map().drawMap();
}
