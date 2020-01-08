/*
 * CompiledNfaEpsilon.h
 *
 *
 * H Ryan Harasimowicz | 9421622
 *
 * css502 | Mickey Phoenix
 * 2015.02.28 | homework (2015.02.18)
 *
 *
 */

#include <utility>
#include <tr1/unordered_map>

#include "FiniteStateMachine.h"


#ifndef COMPILEDNFAEPSILON_H_
#define COMPILEDNFAEPSILON_H_

typedef std::pair<int, std::string> keyPair;

// -----------------------------------------------------------------------------
// hash function to deal with pair
inline size_t keyPair_hash(const keyPair & kp) {
	return std::hash<int>()(kp.first) ^ std::hash<std::string>()(kp.second);
}

// -----------------------------------------------------------------------------
// hash function to deal with set
inline size_t keySet_hash(const std::unordered_set<int> & ks) {
	size_t hashVal = 0;
	for (const auto& sIt : ks) {
		hashVal = hashVal ^ std::hash<int>()(sIt);
	}
	return hashVal;
}

class CompiledNfaEpsilon {
public:

	CompiledNfaEpsilon(FiniteStateMachine*);
	virtual ~CompiledNfaEpsilon();

	bool evaluateNfa(std::string);
	FiniteStateMachine* NfaEpsilonToDfa(FiniteStateMachine*);

private:
	void cycleNfaEStates(std::unordered_set<int>&, std::unordered_set<int>&, std::unordered_set<int>&);
	void addEpsClosure(std::unordered_set<int>&, const std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>&);
	std::unordered_set<int> initStartStates(const FiniteStateMachine*, const std::tr1::unordered_multimap<keyPair, int, decltype(&keyPair_hash)>&);
	void buildDfaNode(int& , std::tr1::unordered_map<int, std::unordered_set<int>>&,
			std::tr1::unordered_map<std::unordered_set<int>, int,decltype(&keySet_hash)>&,
			std::unordered_set<int>&, FiniteStateMachine*&, FiniteStateMachine*&);

	std::unordered_set <std::string> getUniqueTransChars(FiniteStateMachine* );
	std::tr1::unordered_multimap<keyPair, int,decltype(&keyPair_hash)> mapTrans (FiniteStateMachine*);

	void printTrans(std::tr1::unordered_multimap <keyPair,int> );
	void printSet(std::unordered_set<int>);
	void prntConversionStates (int, std::unordered_set<int>);

	void chkSetGoal(const std::unordered_set<int>&,
			const std::unordered_set<int>&, int, std::unordered_set<int>&);
	std::string CharToString(char);
	FiniteStateMachine* FSM;

};

#endif /* COMPILEDNFAEPSILON_H_ */
