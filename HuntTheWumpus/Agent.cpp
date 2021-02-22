#include "Agent.h"
#include <iostream>
#include <queue>

using namespace AgentNS;
using namespace GameNS;
using namespace RouteNS;
using namespace std;

Agent::Agent() {
	initialiseKnowledgeBase();
}

/*
Give the agent the rules of the game.
*/
void Agent::initialiseKnowledgeBase() {
	// Set location to the enterance.
	currentLocation = 0;

	// Set all contents literals to unknown.
	for (int i = EMPTY; i < TRAP + 16; i++) {
		unknownLiterals.push_back(i);
	}

	// Add the rules of the game to known clauses.
	setGuaranteedContents();
	for (int i = 0; i < 16; i++) {
		squareContainsOneThing(i);
		setUniqueContents(i);
		associateContentsWithSenses(i);
		associateSensesWithContents(i);
	}
}

/*
Add clauses saying there is at least one Wumpus/gold/exit.
*/
void Agent::setGuaranteedContents() {
	list<int> wumpusList;
	list<int> goldList;
	list<int> exitList;
	for (int i = 0; i < 16; i++) {
		wumpusList.push_back(WUMPUS + i);
		goldList.push_back(GOLD + i);
		exitList.push_back(EXIT + i);
	}

	knownClauses.push_back(wumpusList);
	knownClauses.push_back(goldList);
	knownClauses.push_back(exitList);
}

/*
Add clauses stating that each square contains exactly one thing.
*/
void Agent::squareContainsOneThing(int square) {
	if (square < 0 || square > 15) {
		throw "Invalid input: square must be between 0 and 15";
	}

	// Each square contains something.
	knownClauses.push_back(list<int>{(WUMPUS + square), (GOLD + square), (TRAP + square), (EXIT + square), (EMPTY + square)});

	// If a square contains one thing, it cannot contain another.
	knownClauses.push_back(list<int>{-(WUMPUS + square), -(GOLD + square)});
	knownClauses.push_back(list<int>{-(WUMPUS + square), -(TRAP + square)});
	knownClauses.push_back(list<int>{-(WUMPUS + square), -(EXIT + square)});
	knownClauses.push_back(list<int>{-(WUMPUS + square), -(EMPTY + square)});

	knownClauses.push_back(list<int>{-(GOLD + square), -(TRAP + square)});
	knownClauses.push_back(list<int>{-(GOLD + square), -(EXIT + square)});
	knownClauses.push_back(list<int>{-(GOLD + square), -(EMPTY + square)});

	knownClauses.push_back(list<int>{-(TRAP + square), -(EXIT + square)});
	knownClauses.push_back(list<int>{-(TRAP + square), -(EMPTY + square)});

	knownClauses.push_back(list<int>{-(EXIT + square), -(EMPTY + square)});
}

/*
Add clauses stating there is only one Wumpus/gold/exit.
*/
void Agent::setUniqueContents(int square) {
	if (square < 0 || square > 15) {
		throw "Invalid input: square must be between 0 and 15";
	}

	for (int j = square + 1; j < 16; j++) {
		knownClauses.push_back(list<int>{-(WUMPUS + square), -(WUMPUS + j)});
		knownClauses.push_back(list<int>{-(GOLD + square), -(GOLD + j)});
		knownClauses.push_back(list<int>{-(EXIT + square), -(EXIT + j)});
	}
}

/*
Add clauses stating that if a square contains the Wumpus/gold/a trap, it and its neighbours are stenchy/glistening/breezy.
*/
void Agent::associateContentsWithSenses(int square) {
	if (square < 0 || square > 15) {
		throw "Invalid input: square must be between 0 and 15";
	}

	if (square % 4 != 3) {
		knownClauses.push_back(list<int>{-(WUMPUS + square), (STENCHY + square + 1)});
		knownClauses.push_back(list<int>{-(GOLD + square), (GLISTENING + square + 1)});
		knownClauses.push_back(list<int>{-(TRAP + square), (BREEZY + square + 1)});
	}

	if (square % 4 != 0) {
		knownClauses.push_back(list<int>{-(WUMPUS + square), (STENCHY + square - 1)});
		knownClauses.push_back(list<int>{-(GOLD + square), (GLISTENING + square - 1)});
		knownClauses.push_back(list<int>{-(TRAP + square), (BREEZY + square - 1)});
	}

	if (square > 3) {
		knownClauses.push_back(list<int>{-(WUMPUS + square), (STENCHY + square - 4)});
		knownClauses.push_back(list<int>{-(GOLD + square), (GLISTENING + square - 4)});
		knownClauses.push_back(list<int>{-(TRAP + square), (BREEZY + square - 4)});
	}

	if (square < 12) {
		knownClauses.push_back(list<int>{-(WUMPUS + square), (STENCHY + square + 4)});
		knownClauses.push_back(list<int>{-(GOLD + square), (GLISTENING + square + 4)});
		knownClauses.push_back(list<int>{-(TRAP + square), (BREEZY + square + 4)});
	}
}

/*
Add clauses stating that if a square is stenchy/glistening/breezy, at least one of it and its neighbours contains the Wumpus/gold/a trap.
*/
void Agent::associateSensesWithContents(int square) {
	if (square < 0 || square > 15) {
		throw "Invalid input: square must be between 0 and 15";
	}

	list<int> stenchyList{ -(STENCHY + square) };
	list<int> glisteningList{ -(GLISTENING + square) };
	list<int> breezyList{ -(BREEZY + square) };

	if (square % 4 != 3) {
		stenchyList.push_back(WUMPUS + square + 1);
		glisteningList.push_back(GOLD + square + 1);
		breezyList.push_back(TRAP + square + 1);
	}

	if (square % 4 != 0) {
		stenchyList.push_back(WUMPUS + square - 1);
		glisteningList.push_back(GOLD + square - 1);
		breezyList.push_back(TRAP + square - 1);
	}

	if (square > 3) {
		stenchyList.push_back(WUMPUS + square - 4);
		glisteningList.push_back(GOLD + square - 4);
		breezyList.push_back(TRAP + square - 4);
	}

	if (square < 12) {
		stenchyList.push_back(WUMPUS + square + 4);
		glisteningList.push_back(GOLD + square + 4);
		breezyList.push_back(TRAP + square + 4);
	}

	knownClauses.push_back(stenchyList);
	knownClauses.push_back(glisteningList);
	knownClauses.push_back(breezyList);
}

/*
Takes a list of new literals known to be true and adds them to the knowledge base. It then checks to see if any additional information can be inferred, which
is also added.
*/
void Agent::updateKnowledgeBase(int location, list<int> literals) {
	if (location < 0 || location > 15) {
		throw "Invalid input: location must be between 0 and 15";
	}

	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		if (*i == 0 || *i < -GOLD_AQUIRED || *i > GOLD_AQUIRED) {
			throw "Invalid input: literal " + to_string(*i) + " does not exist";
		}
	}

	updateVisited(location);

	// Add new known literals.
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		if (find(knownLiterals.begin(), knownLiterals.end(), *i) == knownLiterals.end()) {
			knownLiterals.push_back(*i);
			unknownLiterals.remove(abs(*i));
			knownClauses = simplifyClauses(knownClauses, *i);
		}
	}

	// Add unit clauses to the knowledge base.
	list<int> unitClauses;
	for (list<list<int>>::iterator i = knownClauses.begin(); i != knownClauses.end(); i++) {
		if (i->size() == 1) {
			unitClauses.push_back(i->front());
		}
	}
	if (!unitClauses.empty()) {
		for (list<int>::iterator i = unitClauses.begin(); i != unitClauses.end(); i++) {
			literals.push_back(*i);
		}
		updateKnowledgeBase(currentLocation, literals);
	}
	else {
		prioritiseUnknownLiterals(literals);
		makeInferences(literals);
	}
}

/*
Updates the current location of the agent, records that the agent has visited the new square and that it has been adjacent to the new
square's neighbours.
*/
void Agent::updateVisited(int location) {
	currentLocation = location;
	if (find(visited.begin(), visited.end(), currentLocation) == visited.end()) {
		visited.push_back(currentLocation);
		if (currentLocation > 3 && find(visitedAdjacent.begin(), visitedAdjacent.end(), currentLocation - 4) == visitedAdjacent.end()) {
			visitedAdjacent.push_back(currentLocation - 4);
		}
		if (currentLocation < 12 && find(visitedAdjacent.begin(), visitedAdjacent.end(), currentLocation + 4) == visitedAdjacent.end()) {
			visitedAdjacent.push_back(currentLocation + 4);
		}
		if (currentLocation % 4 != 3 && find(visitedAdjacent.begin(), visitedAdjacent.end(), currentLocation + 1) == visitedAdjacent.end()) {
			visitedAdjacent.push_back(currentLocation + 1);
		}
		if (currentLocation % 4 != 0 && find(visitedAdjacent.begin(), visitedAdjacent.end(), currentLocation - 1) == visitedAdjacent.end()) {
			visitedAdjacent.push_back(currentLocation - 1);
		}
	}
}

/*
Removes a newly known literal from the set of clauses. This improves run time as the knowledge base being operated on is smaller. However, it does not allow for literals
changing from true to false or vice versa as information about the literal is lost. This is acceptable in this case as the map does not change mid game.
*/
list<list<int>> Agent::simplifyClauses(list<list<int>> clauses, int l) {
	list<list<int>>::iterator i = clauses.begin();
	while (i != clauses.end()) {
		// If one literal in a clause is true, the whole clause is always true. It therefore tells us nothing and can be removed.
		if (find(i->begin(), i->end(), l) != i->end()) {
			i = clauses.erase(i);
		}

		// If one literal in a clause is false, at least one of the others must be true. This corresponds to a clause without the false literal.
		else if (find(i->begin(), i->end(), -l) != i->end()) {
			i->remove(-l);
			if (i->empty()) {
				i = clauses.erase(i);
			}
			else {
				i++;
			}
		}

		else {
			i++;
		}
	}
	return clauses;
}

/*
Reorders the unknown literals such that guesses which are more likely to be successful are attempted first.
*/
void Agent::prioritiseUnknownLiterals(list<int> literals) {
	list<int> prioritisedLiterals;
	list<int> unprioritisedLiterals = unknownLiterals;
	list<int> prioritisedLocations = locationPriorities();

	bool stenchFound = false;
	bool glistenFound = false;
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		if ((*i >= STENCHY && *i < GLISTENING) || (*i >= WUMPUS && *i < GOLD)) {
			stenchFound = true;
		}
		else if ((*i >= GLISTENING && *i < BREEZY) || (*i >= GOLD && *i < TRAP)) {
			glistenFound = true;
		}
	}
	// If the current square is stenchy, prioritise guessing the location of the Wumpus.
	if (stenchFound) {
		for (list<int>::iterator j = prioritisedLocations.begin(); j != prioritisedLocations.end(); j++) {
			if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + WUMPUS) != unprioritisedLiterals.end()) {
				prioritisedLiterals.push_front(*j + WUMPUS);
				unprioritisedLiterals.remove(*j + WUMPUS);
			}
		}
	}
	
	// If the current square is glistening, prioritise guessing the location of the gold.
	if (glistenFound) {
		for (list<int>::iterator j = prioritisedLocations.begin(); j != prioritisedLocations.end(); j++) {
			if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + GOLD) != unprioritisedLiterals.end()) {
				prioritisedLiterals.push_front(*j + GOLD);
				unprioritisedLiterals.remove(*j + GOLD);
			}
		}
	}

	// Add the remaining unknown literals.
	for (list<int>::iterator j = prioritisedLocations.begin(); j != prioritisedLocations.end(); j++) {
		if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + WUMPUS) != unprioritisedLiterals.end()) {
			prioritisedLiterals.push_back(*j + WUMPUS);
			unprioritisedLiterals.remove(*j + WUMPUS);
		}
		if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + GOLD) != unprioritisedLiterals.end()) {
			prioritisedLiterals.push_back(*j + GOLD);
			unprioritisedLiterals.remove(*j + GOLD);
		}
		if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + TRAP) != unprioritisedLiterals.end()) {
			prioritisedLiterals.push_back(*j + TRAP);
			unprioritisedLiterals.remove(*j + TRAP);
		}
		if (find(unprioritisedLiterals.begin(), unprioritisedLiterals.end(), *j + EMPTY) != unprioritisedLiterals.end()) {
			prioritisedLiterals.push_back(*j + EMPTY);
			unprioritisedLiterals.remove(*j + EMPTY);
		}
	}

	unknownLiterals = prioritisedLiterals;
}

/*
Returns a list of locations in order of distance away. Distance to a location is measured in the number of actions required to reach it.
*/
list<int> Agent::locationPriorities() {
	list<int> prioritisedLocations;
	list<int> newLocations;

	prioritisedLocations.push_back(currentLocation);
	newLocations.push_back(currentLocation);
	while (prioritisedLocations.size() != 16) {
		list<int> nextNewLocations;
		for (list<int>::iterator i = newLocations.begin(); i != newLocations.end(); i++) {
			if (*i % 4 != 3 && find(prioritisedLocations.begin(), prioritisedLocations.end(), *i + 1) == prioritisedLocations.end()) {
				prioritisedLocations.push_back(*i + 1);
				nextNewLocations.push_back(*i + 1);
			}
			if (*i % 4 != 0 && find(prioritisedLocations.begin(), prioritisedLocations.end(), *i - 1) == prioritisedLocations.end()) {
				prioritisedLocations.push_back(*i - 1);
				nextNewLocations.push_back(*i - 1);
			}
			if (*i > 3 && find(prioritisedLocations.begin(), prioritisedLocations.end(), *i - 4) == prioritisedLocations.end()) {
				prioritisedLocations.push_back(*i - 4);
				nextNewLocations.push_back(*i - 4);
			}
			if (*i < 12 && find(prioritisedLocations.begin(), prioritisedLocations.end(), *i + 4) == prioritisedLocations.end()) {
				prioritisedLocations.push_back(*i + 4);
				nextNewLocations.push_back(*i + 4);
			}
		}
		newLocations = nextNewLocations;
	}

	return prioritisedLocations;
}

/*
Guesses assignments for each of the unknown literals and tries to find a model in which the assignment is valid. If it can't the opposite
assignment must be true and so is added to the knowledge base.
*/
void Agent::makeInferences(list<int> literals) {
	for (list<int>::iterator i = unknownLiterals.begin(); i != unknownLiterals.end(); i++) {
		list<int> newUnknownLiterals = unknownLiterals;
		list<int> newKnownLiterals = knownLiterals;

		newUnknownLiterals.remove(*i);
		newKnownLiterals.push_back(*i);

		cout << "GUESSING " << *i << ": ";
		if (!DPLL(knownClauses, newUnknownLiterals, newKnownLiterals)) {
			literals.push_back(-*i);
			updateKnowledgeBase(currentLocation, literals);
			break;
		}

		newKnownLiterals.remove(*i);
		newKnownLiterals.push_back(-*i);

		cout << "GUESSING " << -*i << ": ";
		if (!DPLL(knownClauses, newUnknownLiterals, newKnownLiterals)) {
			literals.push_back(*i);
			updateKnowledgeBase(currentLocation, literals);
			break;
		}
	}
}

/*
DPLL algorithm for making inferences. It takes a partial model made up of a smaller partial model which is known to satisfy the set of knowledge base and
a single guessed literal and checks if this satisfies the knowledge base. If it does, it guesses another and calls itself with. If not, it checks if the
guessed literal can be given the opposite sign, again calling itself recursively. If neither sign can give a valid model in the recursive steps, the
knowledge base cannot be satisfied with the guessed literal. This implies that the literal has to have the opposite sign, so this can be added to the
knowledge base.
*/
bool Agent::DPLL(list<list<int>> clauses, list<int> literals, list<int> partialModel) {
	// If the model already satisfies/doesn't satisfy the knowledge base, the search can end.
	int modelBool = checkModel(clauses, partialModel);
	if (modelBool == TRUE_INT) {
		cout << "true\n";
		return true;
	}
	else if (modelBool == FALSE_INT) {
		cout << "false\n";
		return false;
	}

	clauses = simplifyClauses(clauses, partialModel.back());

	// Check pure literals.
	int pureLiteral = findPureLiteral(clauses, literals);
	if (pureLiteral != 0) {
		list<int> newLiterals = literals;
		newLiterals.remove(pureLiteral);
		
		list<int> newPartialModel = partialModel;
		newPartialModel.push_back(pureLiteral);
		
		return DPLL(clauses, newLiterals, newPartialModel);
	}

	// Check unit clauses.
	int unitClause = findUnitClause(clauses);
	if (unitClause != 0) {
		list<int> newLiterals = literals;
		newLiterals.remove(unitClause);

		list<int> newPartialModel = partialModel;
		newPartialModel.push_back(unitClause);

		return DPLL(clauses, newLiterals, newPartialModel);
	}

	// Check remaining literals.
	list<int> newLiterals = literals;
	list<int> trueNewPartialModel = partialModel;
	list<int> falseNewPartialModel = partialModel;
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		if (*i >= EMPTY && *i < SHOT_HIT) {
			newLiterals.remove(*i);
			trueNewPartialModel.push_back(*i);
			falseNewPartialModel.push_back(-*i);
			break;
		}
	}

	return DPLL(clauses, newLiterals, trueNewPartialModel) || DPLL(clauses, newLiterals, falseNewPartialModel);
}

/*
Checks if each clause in the knowledge base is satisfied by the partial model. If any clause is not satisfied, the model does not satisfy the knowledge
base. If all clauses as true, the model always does.
*/
int Agent::checkModel(list<list<int>> clauses, list<int> partialModel) {
	bool modelTrue = true;
	for (list<list<int>>::iterator i = clauses.begin(); i != clauses.end(); i++) {
		int clauseBool = checkClause(*i, partialModel);
		if (clauseBool == FALSE_INT) {
			return FALSE_INT;
		}
		else if (clauseBool == NULL_INT) {
			modelTrue = false;
		}
	}
	if (modelTrue) {
		return TRUE_INT;
	}
	return NULL_INT;
}

/*
Checks if a partial model satisfies an individual clause from the set of clauses. If a literal is found in the clause, the whole clause always evaluates
to true. The clause is only false if all negative literals are found, so if any is not found, null is returned. If all are found, false is returned.
*/
int Agent::checkClause(list<int> clause, list<int> partialModel) {
	// Check if the clause is true.
	for (list<int>::iterator i = clause.begin(); i != clause.end(); i++) {
		if (find(partialModel.begin(), partialModel.end(), *i) != partialModel.end()) {
			return TRUE_INT;
		}
	}

	// Check if the clause is unknown.
	for (list<int>::iterator i = clause.begin(); i != clause.end(); i++) {
		if (find(partialModel.begin(), partialModel.end(), -*i) == partialModel.end()) {
			return NULL_INT;
		}
	}

	return FALSE_INT;
}

/*
If a literal has the same sign in every clause in which it appears which is not already true, it can be assigned such that those clauses are true. Any model
which satisfies the knowledge base with the opposite assignment also satisfies it with this assignment, so only this needs exploring.
*/
int Agent::findPureLiteral(list<list<int>> clauses, list<int> literals) {
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		// Record the sign of the first instance of the literal.
		int pure = 0;

		for (list<list<int>>::iterator j = clauses.begin(); j != clauses.end(); j++) {
			// Check for the positive literal.
			if (find(j->begin(), j->end(), *i) != j->end()) {
				if (pure == 0) {
					pure = 1;
				}
				else if (pure == -1) {
					pure = 0;
					break;
				}
			}

			// Check for the negative literal.
			else if (find(j->begin(), j->end(), -*i) != j->end()) {
				if (pure == 0) {
					pure = -1;
				}
				else if (pure == 1) {
					pure = 0;
					break;
				}
			}
		}
		
		if (pure == 1) {
			return *i;
		}
		else if (pure == -1) {
			return -*i;
		}
	}

	return 0;
}

/*
Checks for a clause containing only one literal. This literal has to be assigned to the value in that clause for the model to satisfy all clauses.
*/
int Agent::findUnitClause(list<list<int>> clauses) {
	for (list<list<int>>::iterator i = clauses.begin(); i != clauses.end(); i++) {
		if (i->size() == 1) {
			return i->front();
		}
	}

	return 0;
}

/*
Returns the next action the agent wishes to take.
*/
int Agent::getAction() {
	bool wumpusDone = false;
	bool goldDone = false;
	int wumpusLocation = -1;
	int goldLocation = -1;
	bool stenchFound = false;
	bool glistenFound = false;

	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		if (*i == SHOT_HIT || *i == -SHOT_HIT) {
			wumpusDone = true;
		}
		else if (*i == GOLD_AQUIRED || *i == -GOLD_AQUIRED) {
			goldDone = true;
		}
		else if (*i >= WUMPUS && *i < WUMPUS + 16) {
			wumpusLocation = *i - WUMPUS;
		}
		else if (*i >= GOLD && *i < GOLD + 16) {
			goldLocation = *i - GOLD;
		}
		else if (*i >= STENCHY && *i < STENCHY + 16) {
			stenchFound = true;
		}
		else if (*i >= GLISTENING && *i < GLISTENING + 16) {
			glistenFound = true;
		}
	}

	// If the Wumpus and gold are done,  escape.
	if (wumpusDone && goldDone) {
		cout << "\nESCAPE\n------\n\n";
		return escape();
	}

	// If the location of the gold is known, but it is not done, pathfind to the gold and collect it.
	if (!goldDone && goldLocation != -1) {
		cout << "\nCOLLECT GOLD\n------------\n\n";
		return collectGold(goldLocation);
	}

	// If the location of the Wumpus is known, but it is not done, pathfind to a square adjacent to it and shoot it.
	if (!wumpusDone && wumpusLocation != -1) {
		cout << "\nSHOOT WUMPUS\n------------\n\n";
		return shootWumpus(wumpusLocation);
	}

	// If a glistening square has been found, explore squares adjacent to those where the gold could be.
	if (!goldDone && glistenFound) {
		cout << "\nFIND GOLD\n---------\n\n";
		return findGold();
	}

	// If a stenchy square has been found, explore squares adjacent to those where the Wumpus could be.
	if (!wumpusDone && stenchFound) {
		cout << "\nFIND WUMPUS\n-----------\n\n";
		return findWumpus();
	}

	// Otherwise, explore unknown cells.
	else {
		cout << "\nEXPLORE\n-------\n\n";
		return explore();
	}

	return 0;
}

/*
If the agent is on the exit, escape. Otherwise pathfind to the exit.
*/
int Agent::escape() {
	if (currentLocation == 0) {
		return ESCAPE;
	}
	else {
		return pathFind(list<int> {0});
	}
}

/*
Pathfind to the gold. If no route is possible, even with assuming squares are safe, store that the gold is inaccessible and find a new
action based on this.
*/
int Agent::collectGold(int location) {
	int action = pathFind(list<int>{location});
	if (action == 0) {
		knownLiterals.push_back(-GOLD_AQUIRED);
		return getAction();
	}
	else {
		return action;
	}
}

/*
If adjacent to the Wumpus, shoot it. Otherwise, pathfind to a square adjacent to the Wumpus. If no route is possible, even with assuming
squares are safe, store that the gold is inaccessible and find a new action based on this.
*/
int Agent::shootWumpus(int location) {
	list<int> goals;
	if (location > 3) {
		if (currentLocation == location - 4) {
			return SHOOT_DOWN;
		}
		goals.push_back(location - 4);
	}
	if (location < 12) {
		if (currentLocation == location + 4) {
			return SHOOT_UP;
		}
		goals.push_back(location + 4);
	}
	if (location % 4 != 0) {
		if (currentLocation == location - 1) {
			return SHOOT_RIGHT;
		}
		goals.push_back(location - 1);
	}
	if (location % 4 != 3) {
		if (currentLocation == location + 1) {
			return SHOOT_LEFT;
		}
		goals.push_back(location + 1);
	}

	int action = pathFind(goals);
	if (action == 0) {
		knownLiterals.push_back(-SHOT_HIT);
		return getAction();
	}
	else {
		return action;
	}
}

/*
Pathfind to locations the gold may be in.
*/
int Agent::findGold() {
	list<int> possibleGold;
	for (int i = 0; i < 16; i++) {
		possibleGold.push_back(i);
	}
	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		if (*i <= -GOLD && *i > -(GOLD + 16)) {
			possibleGold.remove(-*i - GOLD);
		}
	}

	int action = pathFind(possibleGold);
	if (action == 0) {
		knownLiterals.push_back(-GOLD_AQUIRED);
		return getAction();
	}
	else {
		return action;
	}
}

/*
Pathfind to locations that may be adjacent to the Wumpus.
*/
int Agent::findWumpus() {
	list<int> possibleWumpus;
	for (int i = 0; i < 16; i++) {
		possibleWumpus.push_back(i);
	}
	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		if (*i <= -WUMPUS && *i > -(WUMPUS + 16)) {
			possibleWumpus.remove(-*i - WUMPUS);
		}
	}

	list<int> goals;
	for (list<int>::iterator i = possibleWumpus.begin(); i != possibleWumpus.end(); i++) {
		if (*i > 3 && find(goals.begin(), goals.end(), *i - 4) == goals.end() && find(visited.begin(), visited.end(), *i - 4) == visited.end()) {
			goals.push_back(*i - 4);
		}
		if (*i < 12 && find(goals.begin(), goals.end(), *i + 4) == goals.end() && find(visited.begin(), visited.end(), *i + 4) == visited.end()) {
			goals.push_back(*i + 4);
		}
		if (*i % 4 != 0 && find(goals.begin(), goals.end(), *i - 1) == goals.end() && find(visited.begin(), visited.end(), *i - 1) == visited.end()) {
			goals.push_back(*i - 1);
		}
		if (*i % 4 != 3 && find(goals.begin(), goals.end(), *i + 1) == goals.end() && find(visited.begin(), visited.end(), *i + 1) == visited.end()) {
			goals.push_back(*i + 1);
		}
	}

	int action = pathFind(goals);
	if (action == 0) {
		knownLiterals.push_back(-SHOT_HIT);
		return getAction();
	}
	else {
		return action;
	}
}

/*
Pathfind to squares with unknown contents.
*/
int Agent::explore() {
	list<int> goals;
	for (int i = 0; i < 16; i++) {
		goals.push_back(i);
	}
	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		if (*i >= EMPTY && *i < TRAP + 16) {
			goals.remove((*i - 1) % 16);
		}
	}

	int action = pathFind(goals);
	if (action == 0) {
		knownLiterals.push_back(-SHOT_HIT);
		knownLiterals.push_back(-GOLD_AQUIRED);
		return getAction();
	}
	else {
		return action;
	}
}

/*
A variant of A* search which attempts to find the shortest route to one of the goal squares. It will never enter a square which is known to be unsafe and
will prioritise routes which contain the fewest squares of unknown safety.
*/
int Agent::pathFind(list<int> goals) {
	auto cmp = [&](Route left, Route right) { return estimateRouteDistance(left, goals) > estimateRouteDistance(right, goals); };
	priority_queue<Route, vector<Route>, decltype(cmp)> safeRoutes(cmp);
	priority_queue<Route, vector<Route>, decltype(cmp)> unsafeRoutes(cmp);
	safeRoutes.push(Route(currentLocation));
	list<int> queued;

	cout << "Goals: ";
	for (list<int>::iterator i = goals.begin(); i != goals.end(); i++) {
		cout << *i << " ";
	}
	cout << "\n";
	while (!safeRoutes.empty()) {
		Route route = safeRoutes.top();
		safeRoutes.pop();
		int square = route.getCurrentSquare();
		cout << "Current " << square << "\n";

		if (find(goals.begin(), goals.end(), square) != goals.end()) {
			cout << "Destination " << square << ", ";
			cout << "Action: " << route.getFirstAction() << "\n\n";
			return route.getFirstAction();
		}

		if (square > 3 && find(queued.begin(), queued.end(), square - 4) == queued.end()) {
			queued.push_back(square - 4);
			if (safeSquare(square - 4) == TRUE_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_UP, square - 4);
				safeRoutes.push(newRoute);
			}
			else if (safeSquare(square - 4) == NULL_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_UP, square - 4);
				unsafeRoutes.push(newRoute);
			}
		}
		if (square % 4 != 0 && find(queued.begin(), queued.end(), square - 1) == queued.end()) {
			queued.push_back(square - 1);
			if (safeSquare(square - 1) == TRUE_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_LEFT, square - 1);
				safeRoutes.push(newRoute);
			}
			else if (safeSquare(square - 1) == NULL_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_LEFT, square - 1);
				unsafeRoutes.push(newRoute);
			}
		}
		if (square < 12 && find(queued.begin(), queued.end(), square + 4) == queued.end()) {
			queued.push_back(square + 4);
			if (safeSquare(square + 4) == TRUE_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_DOWN, square + 4);
				safeRoutes.push(newRoute);
			}
			else if (safeSquare(square + 4) == NULL_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_DOWN, square + 4);
				unsafeRoutes.push(newRoute);
			}
		}
		if (square % 4 != 3 && find(queued.begin(), queued.end(), square + 1) == queued.end()) {
			queued.push_back(square + 1);
			if (safeSquare(square + 1) == TRUE_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_RIGHT, square + 1);
				safeRoutes.push(newRoute);
			}
			else if (safeSquare(square + 1) == NULL_INT) {
				Route newRoute = route;
				newRoute.update(MOVE_RIGHT, square + 1);
				unsafeRoutes.push(newRoute);
			}
		}

		if (safeRoutes.empty() && !unsafeRoutes.empty()) {
			safeRoutes.push(unsafeRoutes.top());
			unsafeRoutes.pop();
		}
	}

	return 0;
}

/*
Returns the minimum movement actions required to reach one of the goals assuming the entire map is empty plus the number of actions already taken.
*/
int Agent::estimateRouteDistance(Route route, list<int> goals) {
	int squareX = route.getCurrentSquare() % 4;
	int squareY = route.getCurrentSquare() / 4;
	int minimumDistance = 6;

	for (list<int>::iterator i = goals.begin(); i != goals.end(); i++) {
		int goalX = *i % 4;
		int goalY = *i / 4;
		int distance = abs(goalX - squareX) + abs(goalY - squareY);
		if (distance < minimumDistance) {
			minimumDistance = distance;
		}
	}

	return minimumDistance + route.getLength();
}

/*
Returns an integer representing the safety of the given square. True implies the square is definitely safe. Null implies the square could be either safe or
unsafe. False implies the square is definitely unsafe. Squares the agent has not been adjacent to are assumed safe for the purpose of pathfinding.
*/
int Agent::safeSquare(int square) {
	// Booleans representing the possible contents of the square.
	bool empty = true;
	bool wumpus = true;
	bool gold = true;
	bool trap = true;
	bool wumpusAlive = true;

	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		if (*i == -(EMPTY + square)) {
			empty = false;
		}
		else if (*i == -(WUMPUS + square)) {
			wumpus = false;
		}
		else if (*i == -(GOLD + square)) {
			gold = false;
		}
		else if (*i == -(TRAP + square)) {
			trap = false;
		}
		else if (*i == SHOT_HIT) {
			wumpusAlive = false;
		}
	}

	// A square is safe if it can't contain a trap and can't contain the wumpus whilst it's alive. If it could contain anything the agent hasn't been
	// adjactent yet.
	if ((!trap && (!wumpus || !wumpusAlive)) || find(visitedAdjacent.begin(), visitedAdjacent.end(), square) == visitedAdjacent.end()) {
		return TRUE_INT;
	}
	// A square is unsafe if it can't contain the gold, a dead wumpus or be empty.
	else if (!empty && !gold && !(wumpus && !wumpusAlive)) {
		return FALSE_INT;
	}
	else {
		return NULL_INT;
	}
}

void Agent::addClause(list<int> clause) {
	knownClauses.push_back(clause);
}

void Agent::addUnknownLiterals(list<int> literals) {
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		unknownLiterals.push_back(*i);
	}
}

list<int> Agent::getKnownLiterals() {
	return knownLiterals;
}

void Agent::printKnowledgeBase(list<list<int>> clauses, list<int> literals, list<int> partialModel) {
	cout << "known: ";
	for (list<int>::iterator i = partialModel.begin(); i != partialModel.end(); i++) {
		cout << *i << " ";
	}
	cout << "\nunknown: ";
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		cout << *i << " ";
	}
	cout << "\nclauses:\n";
	for (list<list<int>>::iterator i = clauses.begin(); i != clauses.end(); i++) {
		for (list<int>::iterator j = i->begin(); j != i->end(); j++) {
			cout << *j << " ";
		}
		cout << "\n";
	}
	cout << "\n\n";
}

void Agent::printKnownLiterals() {
	cout << "Known: ";
	for (list<int>::iterator i = knownLiterals.begin(); i != knownLiterals.end(); i++) {
		cout << *i << " ";
	}
	cout << "\n\n";
}
