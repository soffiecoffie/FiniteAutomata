#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H
#include <vector>
#include <string>

struct State
{
	bool accepting = 0;					//променлива, която казва дали състоянието е финално
	std::vector<std::vector<State*>> nextState;		//масив, който съдържа всички достижими състояния за една стъпка от текущото
};

class FiniteAutomata
{
	std::vector<char> alphabet;							//азбуката на автомата
	std::vector<State*> states;							//масив от всички състояния
	std::vector<State*> acceptingStates;						//масив от всички финални състояния
	std::vector<State*> start;							//масив от всички начални състояние
	bool emptyStrTransition = false; //add check in the find word so you use the last ind as an empt str

	//	State start;	//начално състояние
	bool areAllCharactersFromAlphabet(const std::string&) const;
	int getCharIndFromAlphabet(const char&) const;
	char getCharFromAlphabetByInd(int) const; 
	int getStateIndFromAllStates(const State&) const;
	bool containsWordFrom(const State*, const std::string&) const;
	bool isStarting(const State*) const;
	void del();
	void copy(const FiniteAutomata&);
public:
	int a = 1;
	FiniteAutomata(const std::vector<char>&, const std::vector<State*>&, const std::vector<State*>&, const std::vector<State*>&);
	FiniteAutomata(const FiniteAutomata&);
	FiniteAutomata() {}; //за интерактивно конструиране no po skoro mai ne
	~FiniteAutomata();

	FiniteAutomata Complement(const FiniteAutomata&) const;
	FiniteAutomata Union(const FiniteAutomata&, const FiniteAutomata&) const;
	FiniteAutomata Intersection(const FiniteAutomata&, const FiniteAutomata&) const;
	void determine();//spelled right?
	FiniteAutomata& operator=(const FiniteAutomata&);

	bool containsWord(const std::string&) const;
	bool isDeterministic() const;
	void getInfo() const;
};

#endif
