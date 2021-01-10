#include "FiniteAutomata.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <string>

bool areAllCharactersFromAlphabet(std::string str, std::vector<char> alphabet)
{
	std::cout << str;
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
int main()
{
	std::vector<char> alphabet = {'a','b'};
	std::vector<State*> states;
	std::vector<State*> accepting;
	std::vector<State*> starting;
	State* q1 = new State(); State* q2 = new State(); State* q3 = new State(); State* q4 = new State(); State* q5 = new State();
	std::vector<State*> a1;  a1.push_back(q2); a1.push_back(q3);
	std::vector<State*> b1;  b1.push_back(q4); 
	std::vector<State*> a2;  a2.push_back(q5);
	std::vector<State*> b2;  
	std::vector<State*> a3;
	std::vector<State*> b3;  b3.push_back(q3);
	std::vector<State*> a4;  a4.push_back(q4);
	std::vector<State*> b4;
	std::vector<State*> a5;
	std::vector<State*> b5;	b5.push_back(q5);
	std::vector<std::vector<State*>> next1; next1.push_back(a1); next1.push_back(b1);
	std::vector<std::vector<State*>> next2; next2.push_back(a2); next2.push_back(b2);
	std::vector<std::vector<State*>> next3; next3.push_back(a3); next3.push_back(b3); 
	std::vector<std::vector<State*>> next4; next4.push_back(a4); next4.push_back(b4); 
	std::vector<std::vector<State*>> next5; next5.push_back(a5); next5.push_back(b5);	

	q1->accepting = 0; q1->nextState = next1;
	q2->accepting = 0; q2->nextState = next2;
	q3->accepting = 1; q3->nextState = next3;
	q4->accepting = 0; q4->nextState = next4;
	q5->accepting = 1; q5->nextState = next5;
	accepting.push_back(q3);
	accepting.push_back(q5);
	starting.push_back(q1);
	states.push_back(q1);
	states.push_back(q2);
	states.push_back(q3);
	states.push_back(q4);
	states.push_back(q5);
	FiniteAutomata fna(alphabet, states, accepting, starting);
	fna.getInfo();
	fna.a = 7;
	//std::cout << "\nDoes the automata contain "<<"\"aabbbbb\"? "<< std::boolalpha << fna.containsWord("aabbbbb")<<std::endl;
	int a = 5, b = 10, c = 15;
	a = b = c;
	//std::cout << a;
	FiniteAutomata f2(fna.Complement(fna));
	f2.getInfo();
	FiniteAutomata f3(f2.Union(f2, fna));
	FiniteAutomata f4(f2.Intersection(f2, f3));
	f3.getInfo();
	f4.getInfo();
//	FiniteAutomata f2(fna);
//	f2.a = 2;
//	fna.Complement(fna);
//	FiniteAutomata f3;
	f2.a = 2;
	//f3.a = 3;
	std::cout << "\n";
//	f2 = fna.Complement(fna);
//	f3 = f3.Union(f2, fna);
	//f3.getInfo();

	std::vector<int> vec{1,2,3,4 };
	int value = 7;// value to be removed
	vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
	std::cout << "\n";

	for (size_t i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";

	comb(vec, 1);
	std::string copy = "1234";
	std::reverse(copy.begin(), copy.end());
	std::cout << "\n" << copy;
	std::cout << "\n";
	return 0;
}
//Containing the empty word??

