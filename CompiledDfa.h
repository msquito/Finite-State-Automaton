/*
 * CompiledDfa.h
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

#ifndef COMPILEDDFA_H_
#define COMPILEDDFA_H_


typedef std::pair<int, std::string> keyPair;

// -----------------------------------------------------------------------------
// hash function to deal with pair
inline size_t DFAkeyPair_hash(const keyPair & kp) {
	return std::hash<int>()(kp.first) ^ std::hash<std::string>()(kp.second);
}

class CompiledDfa {
public:
	CompiledDfa(FiniteStateMachine*);
	virtual ~CompiledDfa();

	bool evaluateDfa(std::string);

private:
	std::tr1::unordered_multimap<keyPair, int, decltype(&DFAkeyPair_hash)> mapTrans (FiniteStateMachine*);

	FiniteStateMachine* FSM;
	std::string CharToString(char);
};

#endif /* COMPILEDDFA_H_ */
