#ifndef FINITE_AUTOMATA_CPP
#define FINITE_AUTOMATA_CPP

#include "FiniteAutomata.h"
#include <iostream>
#include <cassert>


FiniteAutomata::FiniteAutomata(const std::vector<char>& alpha, const std::vector<State>& states, const std::vector<State>& accepting, const State& st)
	: alphabet(alpha), states(states), acceptingStates(accepting), start(st)
{
}

void FiniteAutomata::getInfo() const
{
	int sSize = states.size();
	std::cout << "The automata is ";
	if (isDetermined()) std::cout << "deterministic with "<< sSize<<" states.\n";
	else std::cout << "nondeterministic with " << sSize << " states.\n";

	std::cout << "Alphabet = {";
	int aSize = alphabet.size();
	for (size_t i = 0; i < aSize - 1; i++)
	{
		std::cout << alphabet[i] << ", ";
	}
	if (aSize > 0)	std::cout << alphabet[aSize - 1];
	std::cout << "}\n";
}

#endif
