/*
 * CompiledNfaEpsilon.cpp
 *
 * H Ryan Harasimowicz | 9421622
 *
 * css502 | Mickey Phoenix
 * 2015.03.17| final assignment (coding style)
 *
 *
 */

#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>
#include <unordered_set>
#include <tr1/unordered_map>

#include "CompiledNfaEpsilon.h"
#include "FiniteStateMachine.h"


// -----------------------------------------------------------------------------
// constructor
CompiledNfaEpsilon::CompiledNfaEpsilon(FiniteStateMachine* fsm) {
	FSM = fsm;
}

// -----------------------------------------------------------------------------
// destructor
CompiledNfaEpsilon::~CompiledNfaEpsilon() {
	// TODO Auto-generated destructor stub
}

// -----------------------------------------------------------------------------
// add epsilon closure to current state
void CompiledNfaEpsilon::addEpsClosure(std::unordered_set<int>& curState, const std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>& transMap) {

	std::queue<int> epsClosureState { };
	std::unordered_set<int> epsClosures { };
	for (const auto& state : curState) {
		epsClosureState.push(state);
	}

	while (!epsClosureState.empty()) {
		if (transMap.find(keyPair((epsClosureState.front()), "0")) != transMap.end()) {
			if (epsClosures.find(epsClosureState.front()) == epsClosures.end()) {
				curState.insert(transMap.find(keyPair(epsClosureState.front(), "0"))->second);
				epsClosures.insert(epsClosureState.front());
				epsClosureState.push(transMap.find(keyPair(epsClosureState.front(), "0"))->second);
			}
		}
		epsClosureState.pop();
	}
}

// -----------------------------------------------------------------------------
// eval input string - return true or false
std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)> CompiledNfaEpsilon::mapTrans(
		FiniteStateMachine* FSM) {
	std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>
			tMap (32, keyPair_hash);
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
// add unique transition characters to set
std::unordered_set <std::string> CompiledNfaEpsilon::getUniqueTransChars(FiniteStateMachine* FSM){
	std::unordered_set<std::string> inputTC { };
	for (std::list<FiniteStateMachine::Transition>::iterator transition =
			FSM->transitions.begin(); transition != FSM->transitions.end(); ++transition) {

		std::string transCharString = CharToString(transition->transitionChar);
		if (transCharString != "0") {
			inputTC.insert(transCharString);
		}
	}
	return inputTC;
}

// -----------------------------------------------------------------------------
// initialize start states
std::unordered_set<int> CompiledNfaEpsilon::initStartStates(const FiniteStateMachine* FSM, const std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>& transMap){


	std::unordered_set<int> srtState { };
	srtState.insert(FSM->startNode);
	keyPair KP(FSM->startNode, "0");
	auto range = transMap.equal_range(KP);

	for (auto state = range.first; state != range.second; ++state) {
		srtState.insert(state->second);
	}
	return srtState;
}


// -----------------------------------------------------------------------------
// cycle states nxt->cur->lst
void CompiledNfaEpsilon::cycleNfaEStates(std::unordered_set<int>& lstState,
		std::unordered_set<int>& curState, std::unordered_set<int>& nxtState){
	lstState = curState;
	curState = nxtState;
	nxtState.clear();
}

// -----------------------------------------------------------------------------
// eval input string - return true or false
bool CompiledNfaEpsilon::evaluateNfa(std::string input) {

	std::unordered_set<int> curState { }, lstState { };
	std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>	transitionsMap = mapTrans(FSM);

	std::unordered_set<int> nxtState = initStartStates(FSM, transitionsMap);

	curState = nxtState;
	nxtState.clear();

	for (std::string::size_type i = 0; i <= input.size(); ++i) {
		std::string transCharString = CharToString(input[i]);
		if (!curState.empty()) {
			addEpsClosure(curState, transitionsMap);
			for (const auto& state : curState) {
				if (transitionsMap.find(keyPair(state, transCharString)) != transitionsMap.end()){
					nxtState.insert(transitionsMap.find(keyPair(state, transCharString))->second);
				}
			}
		} else {
			return false;
		}
		cycleNfaEStates(lstState, curState, nxtState);
	}
	for (const auto& state: lstState){
		if (FSM->goalNodes.find(state) != FSM->goalNodes.end()) {
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------
// convert char to string for keyPair love...
std::string CompiledNfaEpsilon::CharToString(char transChar) {
	std::stringstream transSS { };
	std::string transString { };
	transSS << transChar;
	transSS >> transString;
	return transString;
}


// -----------------------------------------------------------------------------
// check nfaE goalSet for current state - add to dfa goalSet if found
void CompiledNfaEpsilon::chkSetGoal(const std::unordered_set<int>& states,
		const std::unordered_set<int>& nfaEGoals, int dfaBldN,
		std::unordered_set<int>& dfaGoals) {
	// set goal node if any of startState qualifies
	for (const auto& nfaEState : states) {
		if (nfaEGoals.find(nfaEState) != nfaEGoals.end()) {
			dfaGoals.insert(dfaBldN);
			break;
		}
	}
}

// -----------------------------------------------------------------------------
//
void CompiledNfaEpsilon::buildDfaNode(int& dfaBldN, std::tr1::unordered_map<int, std::unordered_set<int>>& d2n,
		std::tr1::unordered_map<std::unordered_set<int>, int,decltype(&keySet_hash)>& n2d,
		std::unordered_set<int>& nfaENxtS, FiniteStateMachine*& dfa, FiniteStateMachine*& nfaE) {

	dfaBldN++;
	d2n.insert(std::pair<int, std::unordered_set<int>>(dfaBldN, nfaENxtS));
	n2d.insert(std::pair<std::unordered_set<int>, int>(nfaENxtS, dfaBldN));

	dfa->nodes.insert(dfaBldN);
	chkSetGoal(nfaENxtS, nfaE->goalNodes, dfaBldN, dfa->goalNodes);
}

// -----------------------------------------------------------------------------
// eval input string - return true or false
FiniteStateMachine* CompiledNfaEpsilon::NfaEpsilonToDfa(FiniteStateMachine* nfaE) {

	int dfaBldN { 0 }, dfaCurN { 0 };

	FiniteStateMachine* dfa = new FiniteStateMachine { };
	dfa->startNode = dfaBldN;

	std::tr1::unordered_map<int, std::unordered_set<int>> dfa2nfaE { };
	std::tr1::unordered_map<std::unordered_set<int>, int, decltype(&keySet_hash)> nfaE2dfa(32, keySet_hash);

	std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>	transitionsMap = mapTrans(FSM);
	std::unordered_set<std::string> inputTransChars = CompiledNfaEpsilon::getUniqueTransChars(FSM);

	std::unordered_set<int> nfaECurS { };
	std::unordered_set<int> nfaENxtS = initStartStates(nfaE, transitionsMap);
	addEpsClosure(nfaENxtS, transitionsMap);

	dfa2nfaE.insert(std::pair<int, std::unordered_set<int>>(dfaBldN, nfaENxtS));
	nfaE2dfa.insert(std::pair<std::unordered_set<int>, int>(nfaENxtS, dfaBldN));

	chkSetGoal(nfaENxtS, nfaE->goalNodes, dfaBldN, dfa->goalNodes);

	nfaECurS = nfaENxtS;
	nfaENxtS.clear();

	dfa->nodes.insert(dfaCurN);

	while (dfa2nfaE.find(dfaCurN) != dfa2nfaE.end()) {
		for (const auto& tChar : inputTransChars) {
			for (const auto& nfaES : nfaECurS) {
				if (transitionsMap.find(keyPair(nfaES, tChar)) != transitionsMap.end()) {
					nfaENxtS.insert(transitionsMap.find(keyPair(nfaES, tChar))->second);
				}
			}

			addEpsClosure(nfaENxtS, transitionsMap);

			if ((!nfaENxtS.empty())	&& ((nfaE2dfa.find(std::unordered_set<int>(nfaENxtS)) == nfaE2dfa.end()))) {
				buildDfaNode(dfaBldN, dfa2nfaE, nfaE2dfa, nfaENxtS, dfa, nfaE);
				dfa->transitions.push_back(FiniteStateMachine::Transition {	dfaCurN, tChar[0], dfaBldN });
			}

			else if ((!nfaENxtS.empty()) && ((nfaE2dfa.find(std::unordered_set<int>(nfaENxtS)) != nfaE2dfa.end()))) {
				dfa->transitions.push_back(FiniteStateMachine::Transition {
					dfaCurN, tChar[0], nfaE2dfa.find(nfaENxtS)->second });
			}
			nfaENxtS.clear();
		}
		dfaCurN++;
		nfaECurS.clear();
		if (dfa2nfaE.find(dfaCurN) != dfa2nfaE.end()) {
			nfaECurS = dfa2nfaE.find(dfaCurN)->second; // already eClosed...
		}
	}
	return dfa;
}
