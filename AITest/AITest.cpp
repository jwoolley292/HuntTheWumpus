#include "pch.h"
#include "CppUnitTest.h"
#include "Agent.h"
#include "Agent.cpp"
#include "Route.h"
#include "Route.cpp"
#include <list>;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AgentNS;
using namespace std;

namespace AITest
{
	TEST_CLASS(AITest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			/*
			Test map:
			| E | G |   |   |
			|   | W |   |   |
			|   |   |   |   |
			|   |   |   |   |

			Seed: 1110
			*/
			
			Agent agent = Agent();
			
			agent.updateKnowledgeBase(0, list<int> {Agent::EXIT, -Agent::STENCHY, Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_DOWN);

			agent.updateKnowledgeBase(4, list<int> {Agent::EMPTY + 4, Agent::STENCHY + 4, -(Agent::GLISTENING + 4), -(Agent::BREEZY + 4)});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_UP);
			
			agent.updateKnowledgeBase(0, list<int> {Agent::EXIT, -Agent::STENCHY, Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_RIGHT);

			agent.updateKnowledgeBase(1, list<int> {Agent::GOLD_AQUIRED});
			agent.updateKnowledgeBase(1, list<int> {Agent::GOLD + 1, Agent::STENCHY + 1, Agent::GLISTENING + 1, -(Agent::BREEZY + 1)});
			Assert::IsTrue(agent.getAction() == Agent::SHOOT_DOWN);

			agent.updateKnowledgeBase(1, list<int> {Agent::SHOT_HIT});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_LEFT);

			agent.updateKnowledgeBase(0, list<int> {Agent::EXIT, -Agent::STENCHY, Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::ESCAPE);
		}

		TEST_METHOD(TestMethod2) {
			/*
			Test Map:
			| E |   |   | W |
			|   |   | G |   |
			| T |   | T |   |
			|   |   |   |   |

			Seed: 30210222
			*/

			Agent agent = Agent();

			agent.updateKnowledgeBase(0, list<int> {Agent::EXIT, -Agent::STENCHY, -Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_DOWN);

			agent.updateKnowledgeBase(4, list<int> {Agent::EMPTY + 4, -(Agent::STENCHY + 4), -(Agent::GLISTENING + 4), Agent::BREEZY + 4});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_UP);

			agent.updateKnowledgeBase(0, list<int> {Agent::EXIT, -Agent::STENCHY, -Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_RIGHT);

			agent.updateKnowledgeBase(1, list<int> {Agent::EMPTY + 1, -(Agent::STENCHY + 1), -(Agent::GLISTENING + 1), -(Agent::BREEZY + 1)});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_DOWN);

			agent.updateKnowledgeBase(5, list<int> {Agent::EMPTY + 5, -(Agent::STENCHY + 5), Agent::GLISTENING + 5, -(Agent::BREEZY + 5)});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_DOWN);

			agent.updateKnowledgeBase(9, list<int> {Agent::EMPTY + 9, -(Agent::STENCHY + 9), -(Agent::GLISTENING + 9), Agent::BREEZY + 9});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_UP);

			agent.updateKnowledgeBase(5, list<int> {Agent::EMPTY + 5, -(Agent::STENCHY + 5), Agent::GLISTENING + 5, -(Agent::BREEZY + 5)});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_RIGHT);

			agent.updateKnowledgeBase(6, list<int> {Agent::GOLD_AQUIRED});
			agent.updateKnowledgeBase(6, list<int> {Agent::GOLD + 6, -(Agent::STENCHY + 6), Agent::GLISTENING + 6, Agent::BREEZY + 6});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_UP);

			agent.updateKnowledgeBase(2, list<int> {Agent::EMPTY + 2, Agent::STENCHY + 2, Agent::GLISTENING + 2, -(Agent::BREEZY + 2)});
			Assert::IsTrue(agent.getAction() == Agent::SHOOT_RIGHT);

			agent.updateKnowledgeBase(2, list<int> {Agent::SHOT_HIT});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_LEFT);

			agent.updateKnowledgeBase(1, list<int> {Agent::EMPTY + 1, -(Agent::STENCHY + 1), -(Agent::GLISTENING + 1), -(Agent::BREEZY + 1)});
			Assert::IsTrue(agent.getAction() == Agent::MOVE_LEFT);

			agent.updateKnowledgeBase(0, list<int> {Agent::EMPTY, -Agent::STENCHY, -Agent::GLISTENING, -Agent::BREEZY});
			Assert::IsTrue(agent.getAction() == Agent::ESCAPE);
		}
	};
}
