﻿#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H
#include <vector>
#include <string>
#include <fstream>

/** @brief структура, която създава състояния за автомати*/
struct State
{
	bool accepting = 0;									///< променлива, която казва дали състоянието е финално
	std::vector<std::vector<State*>> nextState;			///< масив, който съдържа всички достижими състояния за една стъпка от текущото състояние
};

/** @brief клас, който създава детерминирани и недетерминирани крайни автомати*/
class FiniteAutomata
{
	std::vector<char> alphabet;						///< азбуката на автомата
	std::vector<State*> states;						///< масив от всички състояния
	std::vector<State*> acceptingStates;			///< масив от всички финални състояния
	std::vector<State*> start;						///< масив от всички начални състояние
	bool emptyStrTransition = false;				///< променлива, която казва дали има епсилон преходи
	int det = false;								///< променлива, която казва дали автомата да се детерминира

	bool areAllCharactersFromAlphabet(const std::string&) const;
	bool isRandSymbolInAlphabet() const;
	int getCharIndFromAlphabet(const char&) const;
	int getStateIndFromAllStates(const State&) const;
	int getCountOfTransitions() const;
	bool containsWordFrom(const State*, const std::string&) const;
	bool isStarting(const State*) const;
	void copyTransitions(State*, State*);
	bool isSink(const State*) const;			 
	bool areThereSinks() const;
	void removeSinkNodes();
	FiniteAutomata makeOneLetterAutomata(char) const;
	std::string getNextAutomata(std::string, int& curInd);
	bool isOperation(char) const;
	void del();
	void copy(const FiniteAutomata&);
	std::vector<std::vector<State*>> getStateCombinations(std::vector<State*> arr, int k);

public:
	FiniteAutomata(const std::vector<char>&, const std::vector<State*>&, const std::vector<State*>&,
				   const std::vector<State*>&, bool det = 0);
	FiniteAutomata(const FiniteAutomata&, bool det = 0);
	/** @brief конструктор по подразбиране или с един параметър*/
	FiniteAutomata(bool det = 0):det(det) {};
	~FiniteAutomata();

	FiniteAutomata Complement() const;
	FiniteAutomata Union(const FiniteAutomata&) const;
	FiniteAutomata Intersection(const FiniteAutomata&) const;
	FiniteAutomata Concatenation(const FiniteAutomata&) const;
	FiniteAutomata KleeneStar() const;
	void removeEpsilon();
	void determine();
	FiniteAutomata regexToAutomata(std::string);
	void printAllSentencesFromRegex(std::string filename, std::string regex);
	FiniteAutomata& operator=(const FiniteAutomata&);

	friend std::istream& operator>>(std::istream&, FiniteAutomata&);
	friend std::ostream& operator<<(std::ostream&, const FiniteAutomata&);

	bool containsWord(const std::string&) const;
	bool isDeterministic() const;
	void getInfo() const;
};

#endif
