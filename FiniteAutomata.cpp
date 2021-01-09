#ifndef FINITE_AUTOMATA_CPP
#define FINITE_AUTOMATA_CPP

#include "FiniteAutomata.h"
#include "Table.h"
#include <iostream>
#include <cassert>


bool FiniteAutomata::areAllCharactersFromAlphabet(const std::string& str) const
{
	int size = str.size();
	int aSize = alphabet.size();
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < aSize; j++)
		{
			if (str[i] == alphabet[j]) break;
			else if (j == (aSize - 1)) return false;
		}
	}
	return true;
}

int FiniteAutomata::getCharIndFromAlphabet(const char& ch) const
{
	std::string s;
	s = ch;

	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (alphabet[i] == ch) return i;
	}
	return -1;											//if the character is not from the alphabet 

}

char FiniteAutomata::getCharFromAlphabetByInd(int ind) const
{
	return alphabet[ind];
}

int FiniteAutomata::getStateIndFromAllStates(const State& st) const
{
	for (size_t i = 0; i < states.size(); i++)
	{
		if (states[i] == &st) return i;
	}
	return 0;
}

FiniteAutomata::FiniteAutomata(const std::vector<char>& alpha, const std::vector<State*>& states, 
								const std::vector<State*>& accepting, const std::vector<State*>& st)
							: alphabet(alpha), states(states), acceptingStates(accepting), start(st)
{
}

/*
FiniteAutomata::FiniteAutomata(const FiniteAutomata& other): alphabet(other.alphabet), states(other.states),
															acceptingStates(other.acceptingStates), start(other.start),
															emptyStrTransition(other.emptyStrTransition)
{
}
*/
FiniteAutomata::FiniteAutomata(const FiniteAutomata& other): alphabet(other.alphabet), emptyStrTransition(other.emptyStrTransition)
{
	std::cout << "copying "<<other.a<< "   ";
	copy(other);
}


FiniteAutomata::~FiniteAutomata()
{
	std::cout << a;
	del();
}

FiniteAutomata FiniteAutomata::Complement(const FiniteAutomata& automata) const
{
	//if it's NFA determine it first!
	FiniteAutomata result(automata);
	for (size_t i = 0; i < result.states.size(); i++)
	{
		if (result.states[i]->accepting == true) result.states[i]->accepting = false;
		else result.states[i]->accepting = true;
	}
	result.a = 3;
	return result;
}



FiniteAutomata FiniteAutomata::Union(const FiniteAutomata& a1, const FiniteAutomata& a2) const
{
	FiniteAutomata result;
	result.states = a1.states;
	result.states.insert(result.states.end(), a2.states.begin(), a2.states.end());

	result.acceptingStates = a1.acceptingStates;
	result.acceptingStates.insert(result.acceptingStates.end(), a2.acceptingStates.begin(), a2.acceptingStates.end());


	for (size_t i = 0; i < a1.alphabet.size(); i++) //not adding the empty str yet
	{
		if (a1.alphabet[i] == '@') break;
		else result.alphabet.push_back(a1.alphabet[i]);
	}

	//alphabet might have @ so the sizes will be all wrong

	int aSize = a2.alphabet.size();
	int aResSize = result.alphabet.size();
	for (int i = 0; i < aSize; i++)
	{
		if (a2.alphabet[i] != '@') { //so i dont add the empty str yet
			for (int j = 0; j < aResSize; j++)
			{
				if (a2.alphabet[i] == result.alphabet[j]) break;
				else if (j == aResSize - 1) {
					result.alphabet.push_back(a2.alphabet[i]);
				}
			}
		}
	}
	//fix when they have empty trans from begining
	FiniteAutomata res(result);
	result.states.clear();
	result.acceptingStates.clear();
	result.start.clear();
	result.alphabet.clear();


	int a1StatesSize = a1.states.size();
	res.alphabet.push_back('@'); //adding the empty str
	res.emptyStrTransition = true;
	//int a2StatesSize = a2.states.size();

	for (int i = 0; i < a1StatesSize; i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();//clear doesnt delete dynamic memory right
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a1.alphabet.size() > j && a1.getCharFromAlphabetByInd(j) == res.alphabet[j])
			{
				final.push_back(temp[j]);
			}
			else if (a1.getCharIndFromAlphabet(res.alphabet[j]) != -1)
			{
				int ind = a1.getCharIndFromAlphabet(res.alphabet[j]);
				final.push_back(temp[ind]);
			}
			else 
			{
				final.push_back(empty);
			}
		}
		res.states[i]->nextState = final;

		temp.clear(); //need i clear it ?
		final.clear();
	}

	for (int i = a1StatesSize; i < res.states.size(); i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();//clear doesnt delete dynamic memory right
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a2.alphabet.size() > j && a2.getCharFromAlphabetByInd(j) == res.alphabet[j])
			{
				final.push_back(temp[j]);
			}
			else if (a2.getCharIndFromAlphabet(res.alphabet[j]) != -1)
			{
				int ind = a2.getCharIndFromAlphabet(res.alphabet[j]);
				final.push_back(temp[ind]);
			}
			else
			{
				final.push_back(empty);
			}
		}
		res.states[i]->nextState = final;

		temp.clear(); //need i clear it ?
		final.clear();
	}

	aResSize = res.alphabet.size();

	//creating a new Start state with eps transitions to the start states of a1 and a2 
	State* st = new State();
	for (size_t i = 0; i < aResSize; i++)
	{
		std::vector<State*> v;
		st->nextState.push_back(v);
		if (i == aResSize - 1)   //last symbol of alphabet is the empty str
		{
			//the new starting state will point towards all starting states of the 2 automata
			//with a transition of empty str
			int ind;
			for (size_t j = 0; j < a1.start.size(); j++)
			{
				ind = a1.getStateIndFromAllStates(*a1.start[j]);
				st->nextState[i].push_back(res.states[ind]);
			}
			for (size_t j = 0; j < a2.start.size(); j++)
			{
				ind = a2.getStateIndFromAllStates(*a2.start[j]) + a1.states.size();
				st->nextState[i].push_back(res.states[ind]);
			}
		}
//		v.clear();
	}

	res.start.push_back(st);
	res.states.push_back(st);


	//You need to determine the automata first!!
	return res;
}

FiniteAutomata FiniteAutomata::Intersection(const FiniteAutomata& a1, const FiniteAutomata& a2) const
{
	//determine first
	return Complement(Union(Complement(a1), Complement(a2)));
}



FiniteAutomata FiniteAutomata::Concatenation(const FiniteAutomata& a1, const FiniteAutomata& a2) const
{
	FiniteAutomata result;
	result.states = a1.states;
	result.states.insert(result.states.end(), a2.states.begin(), a2.states.end());

	result.start = a1.start;
	result.acceptingStates = a2.acceptingStates;

	for (size_t i = 0; i < a1.alphabet.size(); i++) //not adding the empty str yet
	{
		if (a1.alphabet[i] == '@') break;
		else result.alphabet.push_back(a1.alphabet[i]);
	}

	//alphabet might have @ so the sizes will be all wrong

	int aSize = a2.alphabet.size();
	int aResSize = result.alphabet.size();
	for (int i = 0; i < aSize; i++)
	{
		if (a2.alphabet[i] != '@') { //so i dont add the empty str yet
			for (int j = 0; j < aResSize; j++)
			{
				if (a2.alphabet[i] == result.alphabet[j]) break;
				else if (j == aResSize - 1) {
					result.alphabet.push_back(a2.alphabet[i]);
				}
			}
		}
	}

	//fix when they have empty trans from begining
	FiniteAutomata res(result);
	result.states.clear();
	result.acceptingStates.clear();
	result.start.clear();
	result.alphabet.clear();


	int a1StatesSize = a1.states.size();
	res.alphabet.push_back('@'); //adding the empty str
	res.emptyStrTransition = true;
	//int a2StatesSize = a2.states.size();

	for (int i = 0; i < a1StatesSize; i++)
	{
		//all accepting states are no more
		res.states[i]->accepting = false;

		//change transitions
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a1.alphabet.size() > j && a1.getCharFromAlphabetByInd(j) == res.alphabet[j])
			{
				final.push_back(temp[j]);
			}
			else if (a1.getCharIndFromAlphabet(res.alphabet[j]) != -1)
			{
				int ind = a1.getCharIndFromAlphabet(res.alphabet[j]);
				final.push_back(temp[ind]);
			}
			else
			{
				final.push_back(empty);
			}
		}
		res.states[i]->nextState = final;

		temp.clear(); //need i clear it ?
		final.clear();
	}

	for (int i = a1StatesSize; i < res.states.size(); i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();//clear doesnt delete dynamic memory right
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a2.alphabet.size() > j && a2.getCharFromAlphabetByInd(j) == res.alphabet[j])
			{
				final.push_back(temp[j]);
			}
			else if (a2.getCharIndFromAlphabet(res.alphabet[j]) != -1)
			{
				int ind = a2.getCharIndFromAlphabet(res.alphabet[j]);
				final.push_back(temp[ind]);
			}
			else
			{
				final.push_back(empty);
			}
		}
		res.states[i]->nextState = final;

		temp.clear(); //need i clear it ?
		final.clear();
	}


	for (size_t i = 0; i < a1.states.size(); i++)
	{
		if (a1.states[i]->accepting) {
			for (size_t j = 0; j < a2.start.size(); j++)
			{
				int ind = a2.getStateIndFromAllStates(*a2.start[j]) + a1.states.size();
				res.states[i]->nextState[alphabet.size() - 1].push_back(res.states[ind]);
			}
		}
	}


	//determine
	//removeEps?
	return result;
}

FiniteAutomata FiniteAutomata::KleeneStar(const FiniteAutomata& q) const
{
	FiniteAutomata result(q);

	//adding an empty empty-string transition for every state
	if (!q.emptyStrTransition)
	{
		result.alphabet.push_back('@');
		result.emptyStrTransition = true;
		for (size_t i = 0; i < result.states.size(); i++)
		{
			std::vector<State*> empty;
			result.states[i]->nextState.push_back(empty);
		}
	}

	//creating a new Start state with eps transition to the start state of q
	State* st = new State();   //is it going to disappear at the end of func? tag
	st->accepting = true;
	int aResSize = result.alphabet.size();

	//adding empty transitions from the start state with every symbol
	//and adding empty-string transitions from the new start to the old start states
	for (size_t i = 0; i < aResSize; i++)
	{
		std::vector<State*> v;
		st->nextState.push_back(v);
		if (i == aResSize - 1)   //last symbol of alphabet is the empty str
		{
			int ind;
			for (size_t j = 0; j < q.start.size(); j++)
			{
				ind = q.getStateIndFromAllStates(*q.start[j]);
				st->nextState[i].push_back(result.states[ind]);
			}
		}
	}
	result.start.clear();

	result.states.push_back(st);
	result.start.push_back(st);
	result.acceptingStates.push_back(st);

	//adding an empty-string transition from all accepting states to the new start state
	for (size_t i = 0; i < result.acceptingStates.size(); i++)
	{
		result.acceptingStates[i]->nextState[aResSize - 1].push_back(result.start[0]);
	}

	//determine?
	//remove emptystr?
	return result;
}



/*
FiniteAutomata FiniteAutomata::Concatenation(FiniteAutomata a1, FiniteAutomata a2) const
{
	//does it delete a1 and a2 at the end?? because they are copies
	for (size_t i = 0; i < a1.acceptingStates.size(); i++)
	{
		a1.acceptingStates[i]->accepting = false;
	}
	a1.acceptingStates.clear();

	for (size_t i = 0; i < a2.acceptingStates.size(); i++)
	{
		a1.acceptingStates.push_back(a2.acceptingStates[i]);
	}

	for (size_t i = 0; i < a1.states.size(); i++)
	{
		a1.states.insert(a1.states.end(), a2.states.begin(), a2.states.end());
	}

	a1.emptyStrTransition = true;

	for (size_t i = 0; i < a1.states.size(); i++)
	{
		for (size_t j = 0; j < a1.; j++)
		{

		}
		a1.states
	}



	return result;
}
*/
void FiniteAutomata::removeEpsilon()
{
	if (!emptyStrTransition) return;

	//I know the empty transition symb is always last in the alphabet
	//Should i make more validations that it really is the last?
	for (size_t i = 0; i < states.size(); i++)
	{
		State* v1 = states[i];
		int lastInd = states[i]->nextState.size() - 1;
		int size = states[i]->nextState[lastInd].size();

		for (size_t j = 0; j < size; j++)
		{
			State* v2 = states[i]->nextState[lastInd][j];

			copyTransitions(v1, v2);
			if (isStarting(v1) && !(isStarting(v2))) start.push_back(v2);
			if (v2->accepting) v1->accepting = true;
		}
		v1->nextState[lastInd].clear();			//clears @ transition array
	}

	alphabet.pop_back();						//removes @ transition
	emptyStrTransition = 0;
}

//not finished
void FiniteAutomata::determine()
{
	if (emptyStrTransition) removeEpsilon();

	FiniteAutomata result;
	std::vector<std::vector<State*>> newStates;
	std::vector<State*> v;
	//add the states in the result states
	newStates.push_back(v); //adding empty set //so if newstates[i].size==0 -> empty set
	//int size = 0;
	int stSize = states.size();
	//for (size_t i = 0; i <= stSize; i++)
	//{
	//	size += combination(stSize, i);
	//}
	for (size_t i = 1; i < states.size(); i++)
	{
		std::vector<std::vector<State*>> ithCombination = getStateCombinations(states, i);
		newStates.insert(newStates.end(), ithCombination.begin(), ithCombination.end());

		ithCombination.clear();
	}
	//delete newStates on the bottom
//	std::vector<std::vector<State*>> finalNewStates;

	states.clear();
	//filling the automata with new states
	for (size_t i = 0; i < newStates.size(); i++)
	{
		states.push_back(new State());
		/* is bellow thing needed? */
		for (size_t j = 0; j < alphabet.size(); j++)
		{
			//запълвам масива с толкова елементи колкото има азбуката ми
			std::vector<State*> v;
			states[i]->nextState.push_back(v);
		}
	}

//	std::vector<State*> newStarts;
	
	int newStSize = newStates.size();
	//choosing new start states
	for (size_t i = 0; i < newStSize; i++)
	{
		if (newStates[i].size() == start.size()) {
			int count = 0;
			for (size_t j = 0; j < newStates[i].size(); j++)
			{
				for (size_t k = 0; k < start.size(); k++)
				{
					if (newStates[i][j] == start[k]) ++count;
				}
			}
			if (count == start.size()) {
				//clear everything in the start array and add the new start state
				start.clear();
				start.push_back(states[i]);//i-th states matches the i-th newState
				//newStarts = newStates[i];//before
				break;
			}
		}
	}

	//fill the accepting arr with the new states
	acceptingStates.clear();
	for (size_t i = 0; i < newStSize; i++)
	{
		for (size_t j = 0; j < newStates[i].size(); j++)
		{
			if (newStates[i][j]->accepting) {
				states[i]->accepting == true;
				acceptingStates.push_back(states[i]);
				break;
			}
		}
	}

	//add transitions
	for (size_t i = 0; i < newStSize; i++)
	{

		for (size_t k = 0; k < alphabet.size(); k++)
		{
			std::vector<State*> v;
			for (size_t j = 0; j < newStates[i].size(); j++)
			{
				for (size_t m = 0; m < newStates[i][j]->nextState[k].size(); m++)
				{
//					if not already in 
					bool repeats = 0;
					for (size_t h = 0; h < v.size(); h++)
					{
						if (newStates[i][j]->nextState[k][m] == v[h]) {
							repeats = true;
							break;
						}
					}
					if(!repeats)
						v.push_back(newStates[i][j]->nextState[k][m]);
				}

				//if v empty
			}
			int ind = -1;
			if (!v.empty()) {
				for (size_t t = 0; t < newStSize; t++)
				{
					if (newStates[t].size() == v.size()) {
						int count = 0;
						for (size_t w = 0; w < newStates[t].size(); w++)
						{
							for (size_t p = 0; p < v.size(); p++)
							{
								//v's elements are all different
								if (newStates[t][w] == v[p]) ++count;
							}
						}
						if (count == v.size()) {
							ind = t;
							break;
						}
					}
				}
				if (ind == -1)std::cout << "bad bad something go wrong fix fix";
				v.clear();
				states[i]->nextState[k].push_back(states[ind]);// do i even have nextstate[k] to even push in it
			}
			else {
				states[i]->nextState[k].push_back(states[0]); //the empty state
			}

		}
	}

	//remove unreachable states
	removeSinkNodes();
	
	//free dynamic memory
	for (size_t i = 0; i < newStSize; i++)
	{
		for (size_t j = 0;  j < newStates[i].size();  j++)
		{
			delete newStates[i][j];
		}
	}
	newStates.clear();
}

FiniteAutomata FiniteAutomata::regexToAutomata(std::string regex)
{
	regex = removeSpaces(regex);

	return FiniteAutomata();
}


FiniteAutomata& FiniteAutomata::operator=(const FiniteAutomata& other)
{
	if (this != &other) {
		del(); 
		alphabet = other.alphabet;
		emptyStrTransition = other.emptyStrTransition;
		copy(other);
		//states = other
	
	}
	return *this;
}

//not finished - should work with Eps trans
//проверява дали от дадено състояние има път с буквите на даден низ до финално състояние
bool FiniteAutomata::containsWordFrom(const State* st,const std::string& str) const
{
	if (str.size() == 0 && st->accepting) return true;		//когато низът е празен съм стигнала последното състояние
	
	char ch = str[0];
	int ind = getCharIndFromAlphabet(ch);
	
	if (ind != -1) {

		if (st->nextState[ind].size() == 0) return false;
		int size = st->nextState[ind].size();
		for (size_t i = 0; i < size; i++)
		{
			if (containsWordFrom(st->nextState[ind][i], str.substr(1, str.size()))) return true;
		}
	}

	return false;
}

bool FiniteAutomata::isStarting(const State* st) const
{
	int size = start.size();
	for (size_t i = 0; i < size; i++)
	{
		if (st == start[i]) return true;
	}
	return false;
}

//copies all transitions from v2 to v1
void FiniteAutomata::copyTransitions(State* v1, State* v2)
{
	for (size_t i = 0; i < v2->nextState.size(); i++)
	{
		v1->nextState[i].insert(v1->nextState[i].end(), v2->nextState[i].begin(), v2->nextState[i].end());
	}
	//should i remove duplicate states in v1 
}

bool FiniteAutomata::isSink(const State* q)
{
	for (size_t i = 0; i < states.size(); i++)
	{
		if (states[i] != q) {
			int size1 = states[i]->nextState.size();
			for (size_t j = 0; j < size1; j++)
			{
				int size2 = states[i]->nextState[j].size();
				for (size_t k = 0; k < size2; k++)
				{
					if (states[i]->nextState[j][k] == q)return true;
				}
			}
		}
	}
	return false;
}

void FiniteAutomata::removeSinkNodes()
{
	int size = states.size();
	for (size_t i = 0; i < size; i++)
	{
		if (isSink(states[i])) {
			if (isStarting(states[i])) {
				start.erase(std::remove(start.begin(), start.end(), states[i]), start.end());   // erase-remove idiom
			}
			if (states[i]->accepting) {
				acceptingStates.erase(std::remove(acceptingStates.begin(), acceptingStates.end(), states[i]),
									  acceptingStates.end());
			}
			State* temp = states[i];
			states.erase(std::remove(states.begin(), states.end(), states[i]), states.end());
			delete temp;	//ok?
			--i;
		}
	}
}

//makes an automata that accepts a one letter word from a given letter
FiniteAutomata FiniteAutomata::makeOneLetterAutomata(char ch) const
{
	FiniteAutomata result;
	result.alphabet.push_back(ch);
	State* q1 = new State();
	State* q2 = new State();
	q2->accepting = true;
	result.states.push_back(q1);
	result.states.push_back(q2);

	result.start.push_back(q1);
	result.acceptingStates.push_back(q2);

	//delete q1;//yes or no
	//delete q2;
	return result;
}

void FiniteAutomata::addTheEmptyWord()
{
	if (!emptyStrTransition) {
		emptyStrTransition = true;
		alphabet.push_back('@');
		for (size_t i = 0; i < states.size(); i++)
		{
			std::vector<State*> empty;
			states[i]->nextState.push_back(empty);
		}
	}

	//creating a new start state that's also accepting so the empty word belongs to the language
	State* st = new State();
	st->accepting = true;
	int aSize = alphabet.size();
	for (size_t i = 0; i < aSize; i++)
	{
		std::vector<State*> v;
		st->nextState.push_back(v);
		if (i == aSize - 1)   //last symbol of alphabet is the empty str
		{
			//the new starting state will point towards all starting states of the original automata
			//with an empty string transition
			for (size_t j = 0; j < start.size(); j++)
			{
				st->nextState[i].push_back(states[j]);
			}
		}
		//		v.clear();
	}

	start.clear();
	start.push_back(st);
	states.push_back(st);
	acceptingStates.push_back(st);
}

void FiniteAutomata::del()
{
	for (size_t i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
	states.clear();
	acceptingStates.clear();
	start.clear();
	alphabet.clear();
}

void FiniteAutomata::copy(const FiniteAutomata& other)
{
	for (size_t i = 0; i < other.states.size(); i++)
	{
		State* s = new State();
		s->accepting = other.states[i]->accepting;
		states.push_back(s);
		if (s->accepting) acceptingStates.push_back(s);
	}
	for (size_t i = 0; i < other.states.size(); i++)
	{
		for (size_t j = 0; j < other.states[i]->nextState.size(); j++)
		{
			std::vector<State*> v;
			for (size_t k = 0; k < other.states[i]->nextState[j].size(); k++)
			{
				int ind = other.getStateIndFromAllStates(*other.states[i]->nextState[j][k]);
				v.push_back(states[ind]);
			}
			states[i]->nextState.push_back(v);
		}
	}
	for (size_t i = 0; i < other.start.size(); i++)
	{
		int ind = other.getStateIndFromAllStates(*other.start[i]);
		start.push_back(states[ind]);
	}
}

std::vector<std::vector<State*>> FiniteAutomata::getStateCombinations(std::vector<State*> arr, int k)
{
	std::vector<std::vector<State*>> result;
	if (k == 0) return result;
	if (k == 1) {
		for (size_t i = 0; i < arr.size(); i++)
		{
			std::vector<State*> v;
			v.push_back(arr[i]);
			result.push_back(v);
		}
		return result;
	}

	int size = arr.size();
	for (size_t i = 0; i < size; i++)
	{
		std::vector<State*> v;
		std::vector<State*> temp;
		v.push_back(arr[i]);
		temp = v;
		for (size_t j = i + 1; j < size; j++)
		{
			v.push_back(arr[j]);
			if (k > 2) {
				std::vector<State*> temp2;
				temp2 = v;
				for (size_t l = j + 1; l < size; l++)
				{
					int save = l;
					for (size_t m = 2; m < k; m++)
					{
						if (save >= size || v.size() > k) break;
						else {
							v.push_back(arr[save++]);
						}
					}
					if (v.size() == k)
						result.push_back(v);
					v = temp2;
				}
			}
			else {
				result.push_back(v);
			}
			v = temp;
		}
	}

	return result;
}

//FiniteAutomata FiniteAutomata::complement() const
//{
//	std::vector<State*> state;
//	return FiniteAutomata(alphabet, );
//}

bool FiniteAutomata::containsWord(const std::string& str) const
{
	if (!areAllCharactersFromAlphabet(str)) return false;
	int size = start.size();
	for (size_t i = 0; i < size; i++)
	{
		if (containsWordFrom(start[i], str)) return true;
	}
	return false;
}

//ако има състояние с 0 или повече от 1 преходи със съответна буква значи е недетерминиран 
bool FiniteAutomata::isDeterministic() const
{
	for (size_t i = 0; i < states.size(); i++)
	{
		for (size_t j = 0; j < states[i]->nextState.size(); j++)
		{
			if (states[i]->nextState[j].size() > 1 || states[i]->nextState[j].size() < 1) return false;
		}
	}

	return true;
}

void FiniteAutomata::getInfo() const
{
	int sSize = states.size();
	std::cout << "> Information\n";
	std::cout << "- The automata is ";
	if (isDeterministic()) std::cout << "deterministic with "<< sSize<<" states.\n";
	else std::cout << "nondeterministic with " << sSize << " states.\n";

	std::cout << "- Alphabet = {";
	int aSize = alphabet.size();
	for (int i = 0; i < (aSize - 1); i++)
	{
		std::cout << alphabet[i] << ", ";
	}
	if (aSize > 0)	std::cout << alphabet[aSize - 1];
	std::cout << "}\n";
	

	if (sSize > 0) {
		std::cout << "\n- Delta (transitions):\n\n";



		std::vector<std::string> s(sSize, "q");
		for (int i = 0; i < sSize; i++)
		{
			s[i].append(std::to_string(i + 1));
			if (isStarting(states[i])) s[i].append(" start");
			if (isStarting(states[i]) && states[i]->accepting) s[i].append(" &");
			if (states[i]->accepting) s[i].append(" accept");
		}

		IColumn** arr = new IColumn * [1 + aSize];
		arr[0] = new Column<std::string>("States", "string", s, sSize);

		std::string name;
		std::string set = "{"; //if its deter make it to be without the set print !!tag
		std::vector<std::string> st;
		for (size_t i = 0; i < aSize; i++)
		{
			name = alphabet[i];

			for (size_t k = 0; k < sSize; k++)
			{
				int size1 = states[k]->nextState[i].size();
				for (size_t j = 0; j < size1; j++)
				{
					set.append("q");
					int index = getStateIndFromAllStates(*states[k]->nextState[i][j]) + 1;
					set.append(std::to_string(index));
					if (j < size1 - 1) set.append(", ");
				}
				set.append("}");
				st.push_back(set);
				set = "{";
			}
			arr[i + 1] = new Column<std::string>(name, "string", st, st.size());
			st.clear();
		}

		Table t(arr, 1 + aSize, "Delta");
		t.print(std::cout);
	}
	else std::cout << "There are no transitions\n";
	



	//add primeren reg izraz
	//moje bi dobavi kolko finalni i kolko nachalni sustoqniq ima
}

#endif
