#include "Agent.h"

using namespace AgentNS;
using namespace GameNS;
using namespace std;

Agent::Agent() {
	initialiseKnowledgeBase();
}

void Agent::initialiseKnowledgeBase() {
	for (int i = 0; i < 16; i++) {
		// Each square contains something.
		knowledgeBase.push_back(list<int>{(WUMPUS + i), (GOLD + i), (TRAP + i), (EXIT + i), (EMPTY + i)});

		// If a square contains one thing, it cannot contain another.
		knowledgeBase.push_back(list<int>{-(WUMPUS + i), -(GOLD + i)});
		knowledgeBase.push_back(list<int>{-(WUMPUS + i), -(TRAP + i)});
		knowledgeBase.push_back(list<int>{-(WUMPUS + i), -(EXIT + i)});
		knowledgeBase.push_back(list<int>{-(WUMPUS + i), -(EMPTY + i)});

		knowledgeBase.push_back(list<int>{-(GOLD + i), -(TRAP + i)});
		knowledgeBase.push_back(list<int>{-(GOLD + i), -(EXIT + i)});
		knowledgeBase.push_back(list<int>{-(GOLD + i), -(EMPTY + i)});

		knowledgeBase.push_back(list<int>{-(TRAP + i), -(EXIT + i)});
		knowledgeBase.push_back(list<int>{-(TRAP + i), -(EMPTY + i)});

		knowledgeBase.push_back(list<int>{-(EXIT + i), -(EMPTY + i)});

		// There is only one Wumpus/gold/exit.
		for (int j = 0; j < 16; j++) {
			if (i != j) {
				knowledgeBase.push_back(list<int>{-(WUMPUS + i), -(WUMPUS + j)});
				knowledgeBase.push_back(list<int>{-(GOLD + i), -(GOLD + j)});
				knowledgeBase.push_back(list<int>{-(EXIT + i), -(EXIT + j)});
			}
		}

		bool rightEdge = i % 4 == 3;
		bool leftEdge = i % 4 == 0;
		bool topEdge = i < 4;
		bool bottomEdge = i > 12;

		// If a square contains the Wumpus/gold/a trap, its neigbours are stenchy/glistening/breezy.
		if (!rightEdge) {
			knowledgeBase.push_back(list<int>{-(WUMPUS + i), (STENCHY + i + 1)});
			knowledgeBase.push_back(list<int>{-(GOLD + i), (GLISTENING + i + 1)});
			knowledgeBase.push_back(list<int>{-(TRAP + i), (BREEZY + i + 1)});
		}

		if (!leftEdge) {
			knowledgeBase.push_back(list<int>{-(WUMPUS + i), (STENCHY + i - 1)});
			knowledgeBase.push_back(list<int>{-(GOLD + i), (GLISTENING + i - 1)});
			knowledgeBase.push_back(list<int>{-(TRAP + i), (BREEZY + i - 1)});
		}

		if (!topEdge) {
			knowledgeBase.push_back(list<int>{-(WUMPUS + i), (STENCHY + i + 4)});
			knowledgeBase.push_back(list<int>{-(GOLD + i), (GLISTENING + i + 4)});
			knowledgeBase.push_back(list<int>{-(TRAP + i), (BREEZY + i + 4)});
		}

		if (!bottomEdge) {
			knowledgeBase.push_back(list<int>{-(WUMPUS + i), (STENCHY + i - 4)});
			knowledgeBase.push_back(list<int>{-(GOLD + i), (GLISTENING + i - 4)});
			knowledgeBase.push_back(list<int>{-(TRAP + i), (BREEZY + i - 4)});
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

		knowledgeBase.push_back(stenchyList);
		knowledgeBase.push_back(glisteningList);
		knowledgeBase.push_back(breezyList);

		// The exit is in square 0.
		knowledgeBase.push_back(list<int> {EXIT});
	}
}