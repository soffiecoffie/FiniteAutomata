#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H
#include <vector>
#include <string>

struct State
{
	bool accepting = 0;									//променлива, която казва дали състоянието е финално
	std::vector<std::vector<State*>> nextState;			//масив, който съдържа всички достижими състояния за една стъпка от текущото
};

class FiniteAutomata
{
	std::vector<char> alphabet;							//азбуката на автомата
	std::vector<State*> states;							//масив от всички състояния
	std::vector<State*> acceptingStates;				//масив от всички финални състояния
	std::vector<State*> start;							//масив от всички начални състояние
	bool emptyStrTransition = false; //add check in the find word so you use the last ind as an empt str

	//	State start;										//начално състояние
	bool areAllCharactersFromAlphabet(const std::string&) const;
	int getCharIndFromAlphabet(const char&) const;
	char getCharFromAlphabetByInd(int) const; //NEEDED????
	int getStateIndFromAllStates(const State&) const;
	bool containsWordFrom(const State*, const std::string&) const;
	bool isStarting(const State*) const;
	void copyTransitions(State*, State*);//test
	bool isSink(const State*);			 //test
	void removeSinkNodes();				 //test
	FiniteAutomata makeOneLetterAutomata(char) const; //test
	void addTheEmptyWord();
	void del();
	void copy(const FiniteAutomata&);
	std::vector<std::vector<State*>> getStateCombinations(std::vector<State*> arr, int k);
public:
	int a = 1;
	//FiniteAutomata(const std::vector<char>&, const std::vector<State>&, const std::vector<State>&, const State&);
	//da ne se podava fin vector? zashtoto moga sama da si go suzdam kato iteriram states i vidq koi sa accepting
	FiniteAutomata(const std::vector<char>&, const std::vector<State*>&, const std::vector<State*>&, const std::vector<State*>&);
	FiniteAutomata(const FiniteAutomata&);
	FiniteAutomata() {}; //за интерактивно конструиране no po skoro mai ne
	~FiniteAutomata();

	//да го направя ли така че да приема един автомат и да връща сечението между текущия с новия
	FiniteAutomata Complement(const FiniteAutomata&) const;
	FiniteAutomata Union(const FiniteAutomata&, const FiniteAutomata&) const;
	FiniteAutomata Intersection(const FiniteAutomata&, const FiniteAutomata&) const;
	FiniteAutomata Concatenation(const FiniteAutomata&, const FiniteAutomata&) const;//test
	FiniteAutomata KleeneStar(const FiniteAutomata&) const;//test
//	FiniteAutomata Concatenation(FiniteAutomata, FiniteAutomata) const;
	void removeEpsilon();//test
	void determine();//test
	FiniteAutomata regexToAutomata(std::string);//test and add bool for determination?
	FiniteAutomata& operator=(const FiniteAutomata&);

	bool containsWord(const std::string&) const;
	bool isDeterministic() const;
	void getInfo() const;
	//итерацията как трябва да бъде? да работи с подадено число и да конкатенира толкова или
};

#endif
