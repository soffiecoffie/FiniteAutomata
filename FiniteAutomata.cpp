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
//	assert(areAllCharactersFromAlphabet(s));

	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (alphabet[i] == ch) return i;
	}
	return -1;
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


	/*

		FIX THIS: ALPHABET MAY BE DIFFERENT FOR A1 AND A2 SO YOU MIGHT HAVE TO ADD TRANSITIONS FOR SYMBOLS THAT
		A1 DOESN'T HAVE and they have to be in the right order!!
		ако а има транзишън за първата буква скипва ако не добавя празен масив
		бтв тр на чисто да започнеш да им ги правиш състоянията
		запази някъде старите
		I MIGHT HAVE ALREADY ADDED @ if a1 or a2 had it so maybe i should think of something else instead of just adding it
		to the alphabet v

	*/

	aResSize = res.alphabet.size();

	//creating a new Start state 
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
