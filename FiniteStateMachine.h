/*
 * FiniteStateMachine.h
 *
 * H Ryan Harasimowicz | 9421622
 *
 * css502 | Mickey Phoenix
 * 2015.02.28 | homework (2015.02.18)
 *
 *
 */


#include <list>
#include <iostream>
#include <unordered_set>

#ifndef FINITESTATEMACHINE_H_
#define FINITESTATEMACHINE_H_

class FiniteStateMachine {
public:
	struct Transition;struct Transition {
		int source;
		char transitionChar;
		int destination;

		Transition(int s, char tC, int d) {
			source = s;
			transitionChar = tC;
			destination = d;
		}

		void tPrint() {
			std::cout << "( " << source << ", " << transitionChar << ", "
					<< destination << " )" << std::endl;
		}
	};

	FiniteStateMachine();
	virtual ~FiniteStateMachine();

	std::unordered_set<int> nodes;
	int startNode = 0;
	std::unordered_set<int> goalNodes;
	std::list<Transition> transitions;
};

#endif /* FINITESTATEMACHINE_H_ */
