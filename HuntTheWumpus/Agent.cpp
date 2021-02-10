#include "Agent.h"

using namespace AgentNS;
using namespace GameNS;
using namespace std;

Agent::Agent() {
	initialiseKnowledgeBase();
}

void Agent::initialiseKnowledgeBase() {
	// Set all literals to unknown.
	for (int i = 1; i < TRAP + 16; i++) {
		unknownLiterals.push_back(i);
	}

	// Fill clauses with the rules of the game.
	for (int i = 0; i < 16; i++) {
		// Each square contains something.
		knownClauses.push_back(list<int>{(WUMPUS + i), (GOLD + i), (TRAP + i), (EXIT + i), (EMPTY + i)});

		// If a square contains one thing, it cannot contain another.
		knownClauses.push_back(list<int>{-(WUMPUS + i), -(GOLD + i)});
		knownClauses.push_back(list<int>{-(WUMPUS + i), -(TRAP + i)});
		knownClauses.push_back(list<int>{-(WUMPUS + i), -(EXIT + i)});
		knownClauses.push_back(list<int>{-(WUMPUS + i), -(EMPTY + i)});

		knownClauses.push_back(list<int>{-(GOLD + i), -(TRAP + i)});
		knownClauses.push_back(list<int>{-(GOLD + i), -(EXIT + i)});
		knownClauses.push_back(list<int>{-(GOLD + i), -(EMPTY + i)});

		knownClauses.push_back(list<int>{-(TRAP + i), -(EXIT + i)});
		knownClauses.push_back(list<int>{-(TRAP + i), -(EMPTY + i)});

		knownClauses.push_back(list<int>{-(EXIT + i), -(EMPTY + i)});

		// There is only one Wumpus/gold/exit.
		for (int j = 0; j < 16; j++) {
			if (i != j) {
				knownClauses.push_back(list<int>{-(WUMPUS + i), -(WUMPUS + j)});
				knownClauses.push_back(list<int>{-(GOLD + i), -(GOLD + j)});
				knownClauses.push_back(list<int>{-(EXIT + i), -(EXIT + j)});
			}
		}

		bool rightEdge = i % 4 == 3;
		bool leftEdge = i % 4 == 0;
		bool topEdge = i < 4;
		bool bottomEdge = i > 12;

		// If a square contains the Wumpus/gold/a trap, its neigbours are stenchy/glistening/breezy.
		if (!rightEdge) {
			knownClauses.push_back(list<int>{-(WUMPUS + i), (STENCHY + i + 1)});
			knownClauses.push_back(list<int>{-(GOLD + i), (GLISTENING + i + 1)});
			knownClauses.push_back(list<int>{-(TRAP + i), (BREEZY + i + 1)});
		}

		if (!leftEdge) {
			knownClauses.push_back(list<int>{-(WUMPUS + i), (STENCHY + i - 1)});
			knownClauses.push_back(list<int>{-(GOLD + i), (GLISTENING + i - 1)});
			knownClauses.push_back(list<int>{-(TRAP + i), (BREEZY + i - 1)});
		}

		if (!topEdge) {
			knownClauses.push_back(list<int>{-(WUMPUS + i), (STENCHY + i + 4)});
			knownClauses.push_back(list<int>{-(GOLD + i), (GLISTENING + i + 4)});
			knownClauses.push_back(list<int>{-(TRAP + i), (BREEZY + i + 4)});
		}

		if (!bottomEdge) {
			knownClauses.push_back(list<int>{-(WUMPUS + i), (STENCHY + i - 4)});
			knownClauses.push_back(list<int>{-(GOLD + i), (GLISTENING + i - 4)});
			knownClauses.push_back(list<int>{-(TRAP + i), (BREEZY + i - 4)});
		}

		// If a square is stenchy/glistening/breezy at least one adjacent square contains the Wumpus/gold/a trap.
		list<int> stenchyList{-(STENCHY + i)};
		list<int> glisteningList{-(GLISTENING + i)};
		list<int> breezyList{-(BREEZY + i)};

		if (!rightEdge) {
			stenchyList.push_back(WUMPUS + i + 1);
			glisteningList.push_back(GOLD + i + 1);
			breezyList.push_back(BREEZY + i + 1);
		}

		if (!leftEdge) {
			stenchyList.push_back(WUMPUS + i - 1);
			glisteningList.push_back(GOLD + i - 1);
			breezyList.push_back(BREEZY + i - 1);
		}

		if (!topEdge) {
			stenchyList.push_back(WUMPUS + i + 4);
			glisteningList.push_back(GOLD + i + 4);
			breezyList.push_back(BREEZY + i + 4);
		}

		if (!bottomEdge) {
			stenchyList.push_back(WUMPUS + i - 4);
			glisteningList.push_back(GOLD + i - 4);
			breezyList.push_back(BREEZY + i - 4);
		}

		knownClauses.push_back(stenchyList);
		knownClauses.push_back(glisteningList);
		knownClauses.push_back(breezyList);
	}
}

/*
Takes a list of new literals known to be true and adds them to the knowledge base. It then checks to see if any additional information can be inferred, which
is also added.
*/
void Agent::updateKnowledgeBase(list<int> literals) {
	// Add new information to the knowledge base.
	for (list<int>::iterator i = literals.begin(); i != literals.end(); i++) {
		knownLiterals.push_back(*i);
		unknownLiterals.remove(abs(*i));
		knownClauses = simplifyClauses(knownClauses, *i);
	}

	// Make inferences.
	for (list<int>::iterator i = unknownLiterals.begin(); i != unknownLiterals.end(); i++) {
		list<int> newUnknownLiterals = unknownLiterals;
		newUnknownLiterals.remove(*i);

		list<int> newKnownLiterals = knownLiterals;
		newKnownLiterals.push_back(*i);

		if (!DPLL(knownClauses, newUnknownLiterals, newKnownLiterals)) {
			updateKnowledgeBase(list<int> {*i});
			break;
		}

		newKnownLiterals.remove(*i);
		newKnownLiterals.push_back(-*i);

		if (!DPLL(knownClauses, newUnknownLiterals, newKnownLiterals)) {
			updateKnowledgeBase(list<int> {*i});
			break;
		}
	}
}

list<list<int>> Agent::simplifyClauses(list<list<int>> clauses, int l) {
	list<list<int>>::iterator i = clauses.begin();
	while (i != clauses.end()) {
		
		// If one literal in a clause is true, the whole clause is always true. It therefore tells us nothing and can be removed.
		if (find(*i->begin(), *i->end(), l) != *i->end()) {
			i = clauses.erase(i);
		}

		// If one literal in a clause is false, at least one of the others must be true. This corresponds to a clause without the false literal.
		else if (find(*i->begin(), *i->end(), -l)) {
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
		return true;
	}
	else if (modelBool == FALSE_INT) {
		return false;
	}

	clauses = simplifyClauses(clauses, *partialModel.end());

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
	newLiterals.remove(literals.front());

	list<int> trueNewPartialModel = partialModel;
	trueNewPartialModel.push_back(literals.front());

	list<int> falseNewPartialModel = partialModel;
	falseNewPartialModel.push_back(literals.front());

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
	for (list<int>::iterator i = partialModel.begin(); i != partialModel.end(); i++) {
		if (find(clause.begin(), clause.end(), *i) != clause.end()) {
			return TRUE_INT;
		}
	}

	// Check if the clause is unknown.
	for (list<int>::iterator i = partialModel.begin(); i != partialModel.end(); i++) {
		if (find(clause.begin(), clause.end(), -*i) == clause.end()) {
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
			if (find(*j->begin(), *j->end(), *i) != *j->end()) {
				if (pure == 0) {
					pure = 1;
				}
				else if (pure == -1) {
					pure = 0;
					break;
				}
			}

			// Check for the negative literal.
			else if (find(*j->begin(), *j->end(), -*i) != *j->end()) {
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
