#include "pch.h"
#include "CppUnitTest.h"
#include "Agent.h"
#include "Agent.cpp"

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
			Agent agent;
			agent.addUnknownLiterals({ 1, 2, 3, 4 });
			agent.addClause({ 1, -2 });
			agent.addClause({ 2, 3 });
			agent.addClause({ 2, 4 });
			agent.updateKnowledgeBase({ -1 });

			list<int> knownLiterals = agent.getKnownLiterals();
			list<int> correctKnownLiterals{ -1, -2, 3, 4 };
			
			Assert::IsTrue(knownLiterals.size() == correctKnownLiterals.size());
			for (list<int>::iterator i = correctKnownLiterals.begin(); i != correctKnownLiterals.end(); i++) {
				Assert::IsTrue(knownLiterals.end() != find(knownLiterals.begin(), knownLiterals.end(), *i));
			}
		}
	};
}
