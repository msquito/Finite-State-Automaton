//============================================================================
// Name        : css502_FSA.cpp
// Author      : H Ryan Harasimowicz | 9421622
// Class	   : css502 | Mickey Phoenix
// Version	   : 2015.02.28 | homework (2015.02.18)
//============================================================================


#include <iostream>
#include <iomanip>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "CompiledDfa.h"
#include "CompiledNfaEpsilon.h"
#include "FiniteStateMachine.h"

void testHarness(FiniteStateMachine* nfaE, FiniteStateMachine* dfa,
		std::list<std::string> pass, std::list<std::string> fail) {

	CompiledNfaEpsilon* NFA 	= new CompiledNfaEpsilon { nfaE };
	CompiledDfa* DFA 			= new CompiledDfa { dfa };

	FiniteStateMachine* dfa2 	= NFA->CompiledNfaEpsilon::NfaEpsilonToDfa(nfaE);
	CompiledDfa* DFA2 			= new CompiledDfa {dfa2};

	int pad { 10 };

	std::cout << "string passes..." << std::endl;
	for (std::list<std::string>::iterator sIt = pass.begin(); sIt != pass.end();
			++sIt) {
		std::cout << std::left;
		std::cout << std::setw(pad) << *sIt;
		if (NFA->CompiledNfaEpsilon::evaluateNfa(*sIt)) {
			std::cout << "nfaE +  ";
		} else {
			std::cout << "nfaE -  ";
		}
		if (DFA->CompiledDfa::evaluateDfa(*sIt)) {
			std::cout << "dfa +  ";
		} else {
			std::cout << "dfa -  ";
		}
		if (DFA2->CompiledDfa::evaluateDfa(*sIt)) {
			std::cout << "nfaE->dfa +  ";
		} else {
			std::cout << "nfaE->dfa -  ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << "string fails..." << std::endl;
	for (std::list<std::string>::iterator sIt = fail.begin(); sIt != fail.end();
			++sIt) {
		std::cout << std::left;
		std::cout << std::setw(pad) << *sIt;
		if (NFA->CompiledNfaEpsilon::evaluateNfa(*sIt)) {
			std::cout << "nfaE +  ";
		} else {
			std::cout << "nfaE -  ";
		}
		if (DFA->CompiledDfa::evaluateDfa(*sIt)) {
			std::cout << "dfa +  ";
		} else {
			std::cout << "dfa -  ";
		}
		if (DFA2->CompiledDfa::evaluateDfa(*sIt)) {
			std::cout << "nfaE->dfa +  ";
		} else {
			std::cout << "nfaE->dfa -  ";
		}
		std::cout << std::endl;
	}

	pass.clear();
	fail.clear();

	delete nfaE;
	delete dfa;

	delete dfa2;
	delete NFA;
	delete DFA;
	delete DFA2;

}

int main() {

// setup fsm_01 for ab* | b*c | a*c*

	FiniteStateMachine* fsm_01 = new FiniteStateMachine { };
	fsm_01->startNode 	= 0;
	fsm_01->nodes 		= {0,1,2,3,4,5,6};
	fsm_01->goalNodes 	= {2,4,5,6};

	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 0, '0', 1 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 0, '0', 3 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 0, '0', 5 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 1, 'a', 2 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 2, 'b', 2 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 3, 'b', 3 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 3, 'c', 4 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 5, 'a', 5 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 5, '0', 6 });
	fsm_01->transitions.push_back(FiniteStateMachine::Transition { 6, 'c', 6 });

	FiniteStateMachine* fsm_02 = new FiniteStateMachine { };
	fsm_02->startNode 	= 0;
	fsm_02->nodes 		= {0,1,2,3,4,5,6,7};
	fsm_02->goalNodes 	= {0,1,3,4,5,6,7};

	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 0, 'a', 1 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 0, 'b', 2 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 0, 'c', 3 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 1, 'a', 4 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 1, 'b', 5 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 1, 'c', 6 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 2, 'b', 2 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 2, 'c', 7 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 3, 'c', 6 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 4, 'a', 4 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 4, 'c', 6 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 5, 'b', 5 });
	fsm_02->transitions.push_back(FiniteStateMachine::Transition { 6, 'c', 6 });

	//  ab* | b*c | a*c*
	std::list<std::string> pass_01 { "a", "ab", "abbb", "c", "bc", "bbbc",
			"aaa", "aac", "c", "ccc", "accc", "aaccc" };
	std::list<std::string> fail_01 { "b", "aab", "aba", "bbb", "cb", "bcb",
			"bbbcc", "ca", "aca", "cac", "bbbg" };

	testHarness(fsm_01, fsm_02, pass_01, fail_01);

	return 0;
}
