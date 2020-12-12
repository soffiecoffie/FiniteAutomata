#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H
#include <vector>
#include <string>

class FiniteAutomata
{
	struct State
	{
		bool accepting;								              	//променлива, която казва дали състоянието е финално
		std::vector<std::vector<State>> nextState;		//масив, който съдържа всички достижими състояния за една стъпка от текущото
	};

	std::vector<char> alphabet;							//азбуката на автомата
	std::vector<State> states;							//масив от всички състояния
	std::vector<State> acceptingStates;			//масив от всички финални състояния
	State start;										        //начално състояние
  
public:
	FiniteAutomata(const std::vector<char>&, const std::vector<State>&, const std::vector<State>&, const State&);
	FiniteAutomata(); 
	
	bool containsWord(const std::string) const;
	bool isDetermined() const;
	void getInfo() const;
};

#endif
