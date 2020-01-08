/*
 * CompiledDfa.cpp
 *
 * H Ryan Harasimowicz | 9421622
 *
 * css502 | Mickey Phoenix
 * 2015.02.28 | homework (2015.02.18)
 *
 *
 */


#include <list>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>
#include <tr1/unordered_map>

#include "CompiledDfa.h"


// -----------------------------------------------------------------------------
// constructor
CompiledDfa::CompiledDfa(FiniteStateMachine* fsm) {
	FSM = fsm;
}

CompiledDfa::~CompiledDfa() {}

// -----------------------------------------------------------------------------
// convert char to string for keyPair love...
std::string CompiledDfa::CharToString(char transitionChar) {
	std::stringstream transitionSS { };
	std::string transitionStr { };
	transitionSS << transitionChar;
	transitionSS >> transitionStr;
	return transitionStr;
}

// -----------------------------------------------------------------------------
// map transitions, return unordered map
std::tr1::unordered_multimap<keyPair, int, decltype(&DFAkeyPair_hash)> CompiledDfa::mapTrans(
		FiniteStateMachine* FSM) {
	std::tr1::unordered_multimap<keyPair, int, decltype(&DFAkeyPair_hash)>
			tMap (32, DFAkeyPair_hash);
	for (std::list<FiniteStateMachine::Transition>::iterator trans =
			FSM->transitions.begin(); trans != FSM->transitions.end(); ++trans) {

		std::string transCharString = CharToString(trans->transitionChar);
		tMap.insert(
				std::pair<keyPair, int>(keyPair(trans->source, transCharString),
						trans->destination));
	}
	return tMap;
}

// -----------------------------------------------------------------------------
//
bool CompiledDfa::evaluateDfa(std::string input) {
	int curState { }, lastState { };
	std::tr1::unordered_multimap<keyPair, int, decltype(&DFAkeyPair_hash)>	transitionsMap = mapTrans(FSM);
	for (std::string::size_type size = 0; size <= input.size(); ++size) {
		std::string transChar = CharToString(input[size]);
		if (curState != -1) {
			if (transitionsMap.find(keyPair(curState, transChar)) != transitionsMap.end()) {
				curState = (transitionsMap.find(keyPair(curState, transChar))->second);
				lastState = curState;
			} else {
				curState = -1;
			}
		} else {
			return false;
		}
	}
	if (FSM->goalNodes.find(lastState) == FSM->goalNodes.end()) {
		return false;
	}
	return true;
}
