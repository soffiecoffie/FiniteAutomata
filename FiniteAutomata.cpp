﻿#ifndef FINITE_AUTOMATA_CPP
#define FINITE_AUTOMATA_CPP

#include "FiniteAutomata.h"
#include "Table.h"
#include <iostream>
#include <cassert>

/** @brief проверява дали всички символи на даден низ принадлежат на азбуката на автомата */
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

/** @brief проверява дали знакът за произволен символ ('?') принадлежи на азбуката */
bool FiniteAutomata::isRandSymbolInAlphabet() const
{
	return areAllCharactersFromAlphabet("j");
}

/** @brief връща индекса на даден символ в азбуката на автомата, ако не принадлежи на азбуката връща -1 */
int FiniteAutomata::getCharIndFromAlphabet(const char& ch) const
{
	std::string s;
	s = ch;

	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (alphabet[i] == ch) return i;
	}
	return -1;							
}

/** @brief връща индексът на дадено състояние от масива states на автомата */
int FiniteAutomata::getStateIndFromAllStates(const State& st) const
{
	for (size_t i = 0; i < states.size(); i++)
	{
		if (states[i] == &st) return i;
	}
	return 0;
}

/** @brief връща броя на всички преходи  */
int FiniteAutomata::getCountOfTransitions() const
{
	int count = 0;
	for (size_t i = 0; i < states.size(); i++)
	{
		for (size_t j = 0; j < states[i]->nextState.size(); j++)
		{
			count += states[i]->nextState[j].size();
		}
	}
	return count;
}

/** @brief конструктор с параметри */
FiniteAutomata::FiniteAutomata(const std::vector<char>& alpha, const std::vector<State*>& states, 
								const std::vector<State*>& accepting, const std::vector<State*>& st, bool det)
							: alphabet(alpha), states(states), acceptingStates(accepting), start(st), det(det)
{
	if (det && !isDeterministic()) determine();
}

/** @brief копи конструктор */
FiniteAutomata::FiniteAutomata(const FiniteAutomata& other, bool det): alphabet(other.alphabet),
								emptyStrTransition(other.emptyStrTransition), det(det)
{
	copy(other);
	if (det && !isDeterministic()) determine();
}

/** @brief деструктор */
FiniteAutomata::~FiniteAutomata()
{
	del();
}

/** @brief връща автомат, който е допълнението на даден автомат */
FiniteAutomata FiniteAutomata::Complement() const
{
	FiniteAutomata result(*this);
	//Ако автоматът е недетерминиран, първо трябва да се детерминира
	if (!result.isDeterministic()) {
		result.determine();
	}
	for (size_t i = 0; i < result.states.size(); i++)
	{
		if (result.states[i]->accepting == true) result.states[i]->accepting = false;
		else result.states[i]->accepting = true;
	}

	if (det && !result.isDeterministic()) result.determine();
	return result;
}

/** @brief връща автомат, който е обединението на 2 дадени автомата */
FiniteAutomata FiniteAutomata::Union(const FiniteAutomata& a2) const
{
	FiniteAutomata a1(*this);
	FiniteAutomata result;
	result.states = a1.states;
	result.states.insert(result.states.end(), a2.states.begin(), a2.states.end());

	result.acceptingStates = a1.acceptingStates;
	result.acceptingStates.insert(result.acceptingStates.end(), a2.acceptingStates.begin(), a2.acceptingStates.end());


	for (size_t i = 0; i < a1.alphabet.size(); i++) //за да не се добави празната дума още
	{
		if (a1.alphabet[i] == '@') break;
		else result.alphabet.push_back(a1.alphabet[i]);
	}

	int aSize = a2.alphabet.size();
	int aResSize = result.alphabet.size();
	for (int i = 0; i < aSize; i++)
	{
		if (a2.alphabet[i] != '@') { //за да не добавя празната дума още
			for (int j = 0; j < aResSize; j++)
			{
				if (a2.alphabet[i] == result.alphabet[j]) break;
				else if (j == aResSize - 1) {
					result.alphabet.push_back(a2.alphabet[i]);
				}
			}
		}
	}

	FiniteAutomata res(result);
	result.states.clear();
	result.acceptingStates.clear();
	result.start.clear();
	result.alphabet.clear();


	int a1StatesSize = a1.states.size();
	res.alphabet.push_back('@'); //вече добавям празната дума
	res.emptyStrTransition = true;

	for (int i = 0; i < a1StatesSize; i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a1.alphabet.size() > j && a1.alphabet[j] == res.alphabet[j])
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

		temp.clear();
		final.clear();
	}

	for (int i = a1StatesSize; i < res.states.size(); i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a2.alphabet.size() > j && a2.alphabet[j] == res.alphabet[j])
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

		temp.clear(); 
		final.clear();
	}

	aResSize = res.alphabet.size();

	//създавам ново начално състояние с епсилон преходи към началните състояния на а1 и а2 
	State* st = new State();
	for (size_t i = 0; i < aResSize; i++)
	{
		std::vector<State*> v;
		st->nextState.push_back(v);
		if (i == aResSize - 1)   //последният символ на азбуката е празната дума
		{
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
	}

	res.start.push_back(st);
	res.states.push_back(st);

	if (det && !res.isDeterministic()) res.determine();
	return res;
}

/** @brief връща автомат, който е сечението на 2 дадени автомата */
FiniteAutomata FiniteAutomata::Intersection(const FiniteAutomata& a2) const
{
	//Intersection(a1, a2) = Complement(Union(Complement(a1), Complement(a2)));

	FiniteAutomata result( ( (this->Complement()).Union(a2.Complement()) ).Complement() );

	if (det && !result.isDeterministic()) result.determine();
	return result;   
}

/** @brief връща автомат, който е конкатенацията на 2 дадени автомата */
FiniteAutomata FiniteAutomata::Concatenation(const FiniteAutomata& a2) const
{
	FiniteAutomata a1(*this);

	FiniteAutomata result;
	result.states = a1.states;
	result.states.insert(result.states.end(), a2.states.begin(), a2.states.end());

	result.start = a1.start;
	result.acceptingStates = a2.acceptingStates;

	for (size_t i = 0; i < a1.alphabet.size(); i++) 
	{
		if (a1.alphabet[i] == '@') break;						//за да не добавя празната дума още
		else result.alphabet.push_back(a1.alphabet[i]);
	}

	int aSize = a2.alphabet.size();
	int aResSize = result.alphabet.size();
	for (int i = 0; i < aSize; i++)
	{
		if (a2.alphabet[i] != '@') {							//за да не добавя празната дума още
			for (int j = 0; j < aResSize; j++)
			{
				if (a2.alphabet[i] == result.alphabet[j]) break;
				else if (j == aResSize - 1) {
					result.alphabet.push_back(a2.alphabet[i]);
				}
			}
		}
	}
	FiniteAutomata res(result);
	result.states.clear();
	result.acceptingStates.clear();
	result.start.clear();
	result.alphabet.clear();


	int a1StatesSize = a1.states.size();
	res.alphabet.push_back('@'); //добавям празната дума вече
	res.emptyStrTransition = true;

	for (int i = 0; i < a1StatesSize; i++)
	{
		//всички приемащи състояния от а1 стават неприемащи
		res.states[i]->accepting = false;

		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a1.alphabet.size() > j && a1.alphabet[j] == res.alphabet[j])
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

		temp.clear();
		final.clear();
	}

	for (int i = a1StatesSize; i < res.states.size(); i++)
	{
		std::vector<std::vector<State*>> temp = res.states[i]->nextState;
		std::vector<std::vector<State*>> final;
		std::vector<State*> empty;

		res.states[i]->nextState.clear();
		for (int j = 0; j < res.alphabet.size(); j++)
		{
			if (a2.alphabet.size() > j && a2.alphabet[j] == res.alphabet[j])
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

		temp.clear(); 
		final.clear();
	}

	for (size_t i = 0; i < a1.states.size(); i++)
	{
		if (a1.states[i]->accepting) {
			for (size_t j = 0; j < a2.start.size(); j++)
			{
				int ind = a2.getStateIndFromAllStates(*a2.start[j]) + a1.states.size();
				res.states[i]->nextState[res.alphabet.size() - 1].push_back(res.states[ind]);
			}
		}
	}

	if (det && !res.isDeterministic()) res.determine();
	return res;
}

/** @brief връща автомат, който е итерацията на даден автомат */
FiniteAutomata FiniteAutomata::KleeneStar() const
{
	FiniteAutomata result(*this);

	//добавям епсилон преход за всяко състояние 
	if (!emptyStrTransition)
	{
		result.alphabet.push_back('@');
		result.emptyStrTransition = true;
		for (size_t i = 0; i < result.states.size(); i++)
		{
			std::vector<State*> empty;
			result.states[i]->nextState.push_back(empty);
		}
	}

	//създавам ново начално състояние с епсилон преход към началното състояние на q
	State* st = new State();   
	st->accepting = true;
	int aResSize = result.alphabet.size();

	//добавям епсилон преход от всички финални състояния към новото начално състояние
	for (size_t i = 0; i < result.acceptingStates.size(); i++)
	{
		result.acceptingStates[i]->nextState[aResSize - 1].push_back(st);
	}

	//добавям празни преходи от началното състояние с всеки символ
	//добявам епислон преход от новото начално към старите начални състояния
	for (size_t i = 0; i < aResSize; i++)
	{
		std::vector<State*> v;
		st->nextState.push_back(v);
		if (i == aResSize - 1)   //последният символ в азбуката е празната дума
		{
			int ind;
			for (size_t j = 0; j < start.size(); j++)
			{
				ind = getStateIndFromAllStates(*start[j]);
				st->nextState[i].push_back(result.states[ind]);
			}
		}
	}
	result.start.clear();

	result.states.push_back(st);
	result.start.push_back(st);
	result.acceptingStates.push_back(st);

	if (det && !result.isDeterministic()) result.determine();
	return result;
}

/** @brief премахва епсилон преходите на текущия автомат */
void FiniteAutomata::removeEpsilon()
{
	if (!emptyStrTransition) return;
	FiniteAutomata helper = *this;

	//Празната дума винаги е последна в азбуката
	for (size_t i = 0; i < states.size(); i++) 
	{
		State* v1 = states[i];
		int lastInd = alphabet.size() - 1;
		int size = states[i]->nextState[lastInd].size();

		for (size_t j = 0; j < size; j++)
		{
			State* v2 = states[i]->nextState[lastInd][j];
			
			//в случай, че v2 има епсилон преход към друго състояние, което има епсилон преходи, трябва първо да 
			//копирам преходите на другото състояние, иначе v2 ще изпусне някои преходи
			if (v2->nextState[lastInd].size() > 0) {
				int size2 = v2->nextState[lastInd].size();

				for (size_t k = 0; k < size2; k++)
				{
					State* v3 = v2->nextState[lastInd][k];
					copyTransitions(v2, v3);
					if (isStarting(v2) && !(isStarting(v3))) start.push_back(v3);
					if (v3->accepting) v2->accepting = true;
				}
			}

			copyTransitions(v1, v2);
			if (isStarting(v1) && !(isStarting(v2))) start.push_back(v2);
			if (v2->accepting) v1->accepting = true;
		}
		v1->nextState[lastInd].clear();			//clears @ transition array
	}

	alphabet.pop_back();						//премахвам празната дума от азбуката
	emptyStrTransition = 0;
}

/** @brief детерминира текущия автомат */
void FiniteAutomata::determine()
{
	if (emptyStrTransition) removeEpsilon();

	FiniteAutomata result;
	std::vector<std::vector<State*>> newStates;
	std::vector<State*> v;

	newStates.push_back(v); //добавям празен масив, който ще играе ролята на празното множество

	int stSize = states.size();
	for (size_t i = 1; i < states.size(); i++)
	{
		std::vector<std::vector<State*>> ithCombination = getStateCombinations(states, i);
		newStates.insert(newStates.end(), ithCombination.begin(), ithCombination.end());

		ithCombination.clear();
	}

	states.clear();
	//запълвам автомата с нови състояния
	for (size_t i = 0; i < newStates.size(); i++)
	{
		states.push_back(new State());
		for (size_t j = 0; j < alphabet.size(); j++)
		{
			//запълвам масива с толкова елементи колкото има азбуката ми
			std::vector<State*> v;
			states[i]->nextState.push_back(v);
		}
	}
	
	int newStSize = newStates.size();
	//избирам новите начални състояния
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
				//изтрива всичко от масива за начални състояния и добавя новото начално състояние
				start.clear();
				start.push_back(states[i]);			//i-тото състояние от states съвпада с i-тото състояние от newState
				break;
			}
		}
	}
	//запълвам масива за финални състояния с новите състояния
	acceptingStates.clear();
	for (size_t i = 0; i < newStSize; i++)
	{
		for (size_t j = 0; j < newStates[i].size(); j++)
		{
			if (newStates[i][j]->accepting) {
				states[i]->accepting = true;
				acceptingStates.push_back(states[i]);
				break;
			}
		}
	}

	//добавям преходите
	for (int i = 0; i < newStSize; i++)
	{
		for (int k = 0; k < alphabet.size(); k++)
		{
			std::vector<State*> v;
			for (int j = 0; j < newStates[i].size(); j++)
			{
				for (int m = 0; m < newStates[i][j]->nextState[k].size(); m++)
				{
					bool repeats = 0;
					for (int h = 0; h < v.size(); h++)
					{
						if (newStates[i][j]->nextState[k][m] == v[h]) {
							repeats = true;
							break;
						}
					}
					if(!repeats)
						v.push_back(newStates[i][j]->nextState[k][m]);
				}
			}
			int ind = -1;
			if (!v.empty()) {
				for (int t = 0; t < newStSize; t++)
				{
					if (newStates[t].size() == v.size()) {
						int count = 0;
						for (int w = 0; w < newStates[t].size(); w++)
						{
							for (int p = 0; p < v.size(); p++)
							{
								//всички елементи във v са различни
								if (newStates[t][w] == v[p]) {
									++count; 
									break;
								}
							}
						} 
						if (count == v.size()) {
							ind = t;
							count = 0;
							break;
						}
					}
				}
				if (ind == -1) std::cout << "bad something went wrong";
				v.clear();
				states[i]->nextState[k].push_back(states[ind]);	
			}
			else {
				states[i]->nextState[k].push_back(states[0]);	//"празното множество"
			}

		}
	}

	//премахвам недостижимите състояния
	removeSinkNodes();
}

/** @brief връща автомат, създаден от даден регулярен израз */
FiniteAutomata FiniteAutomata::regexToAutomata(std::string regex)
{
	regex = removeSpaces(regex);
	FiniteAutomata result;
	FiniteAutomata rightA;

	int curInd = regex.size() - 1;
	char curCh = regex[curInd];
	std::string right;
	if (curCh == '*') {
		--curInd;
		right = getNextAutomata(regex, curInd);
		rightA = regexToAutomata(right).KleeneStar();
	}
	else {
		right = getNextAutomata(regex, curInd);

		if (right.size() == 1) {
			rightA = makeOneLetterAutomata(right[0]);
		}
		else {
			rightA = regexToAutomata(right);
		}
	}
	if (curInd < 0) 	
		return rightA;
	
	curCh = regex[curInd];

	std::string str = regex;
	std::string left;
	do {
		switch (curCh)
		{
		case '+':
			str.erase(str.begin() + curInd, str.end());
			--curInd;
			result = regexToAutomata(str).Union(rightA);
			return result;
		case '&':
			str.erase(str.begin() + curInd, str.end());
			--curInd;
			result = regexToAutomata(str).Intersection(rightA);
			return result;
		case '.':
			result = rightA;
			do {
				--curInd;
				left = getNextAutomata(regex, curInd);
				result = regexToAutomata(left).Concatenation(result);
				if (curInd < 0) return result;
				curCh = regex[curInd];
			} while (curCh == '.');
			rightA = result;
			break;
		}
	} while (curInd >= 0);

	return result;
}

/** @brief  извежда всички редове, които се разпознават от регулярния израз  */
void FiniteAutomata::printAllSentencesFromRegex(std::string filename, std::string regex)
{
	std::ifstream in(filename);
	std::string temp;

	if (!in) std::cout << "Can't open file " << filename << "!\n";
	else {
		int size;
		in >> size;
		in.ignore(1, '\n');
		FiniteAutomata a;
		a = a.regexToAutomata(regex);

		for (int i = 0; i < size; i++)
		{
			getline(in, temp, '\n');
			if (a.containsWord(temp))std::cout << temp << "\n";
		}
	}
}

/** @brief оператор = */
FiniteAutomata& FiniteAutomata::operator=(const FiniteAutomata& other)
{
	if (this != &other) {
		del(); 
		alphabet = other.alphabet;
		emptyStrTransition = other.emptyStrTransition;
		copy(other);	
	}
	return *this;
}

/** @brief проверява дали от дадено състояние има път с буквите на даден низ до финално състояние */
bool FiniteAutomata::containsWordFrom(const State* st, const std::string& str) const
{
	if (str.size() == 0 && st->accepting) return true;

	char ch = str[0];
	int ind = getCharIndFromAlphabet(ch);

	if (ind != -1) {							//Проверявам преходите с текущата буква, ако принадлежи на азбуката
		int size = st->nextState[ind].size();
		for (size_t i = 0; i < size; i++)
		{
			if (containsWordFrom(st->nextState[ind][i], str.substr(1, str.size()))) return true;	 
		}
	}
	if (isRandSymbolInAlphabet()) {				//Проверявам преходите със символа за произволен символ, ако е в азбуката
		ind = getCharIndFromAlphabet('?');
		int size = st->nextState[ind].size();
		for (size_t i = 0; i < size; i++)
		{
			if (containsWordFrom(st->nextState[ind][i], str.substr(1, str.size()))) return true;
		}
	}
	if (emptyStrTransition) {					//Проверявам епсилон преходите, ако има такива
		ind = alphabet.size() - 1;
		int size = st->nextState[ind].size();
		for (size_t i = 0; i < size; i++)
		{
			if (ind != -1 && containsWordFrom(st->nextState[ind][i], str)) return true;
		}
	}
	return false;
}

/** @brief проверява дали дадено състояние е начално */
bool FiniteAutomata::isStarting(const State* st) const
{
	int size = start.size();
	for (size_t i = 0; i < size; i++)
	{
		if (st == start[i]) return true;
	}
	return false;
}

/** @brief добавя всички преходи на дадено състояние в друго дадено състояние */
void FiniteAutomata::copyTransitions(State* v1, State* v2)
{
	bool flag = false;
	for (size_t i = 0; i < v2->nextState.size(); i++)
	{
		for (size_t j = 0; j < v2->nextState[i].size(); j++)
		{
			//проверявам дали състоянието не е вътре вече
			for (size_t k = 0; k < v1->nextState[i].size(); k++)
			{
				if (v1->nextState[i][k] == v2->nextState[i][j]) flag = true;
			}
			//ако състоянието не се повтаря ще го добавя, иначе го пропускам
			if(!flag)
				v1->nextState[i].push_back(v2->nextState[i][j]);
			else {
				flag = false;
			}
		}
	}
}

/** @brief проверява дали дадено състояние е недостижимо */
bool FiniteAutomata::isSink(const State* q) const
{
	if (isStarting(q))return false;
	for (size_t i = 0; i < states.size(); i++)
	{
		if (states[i] != q) {
			int size1 = states[i]->nextState.size();
			for (size_t j = 0; j < size1; j++)
			{
				int size2 = states[i]->nextState[j].size();
				for (size_t k = 0; k < size2; k++)
				{
					if (states[i]->nextState[j][k] == q)return false;
				}
			}
		}
	}

	return true;
}

/** @brief проверява дали има недостижими състояния в автомата */
bool FiniteAutomata::areThereSinks() const
{
	for (size_t i = 0; i < states.size(); i++)
	{
		if (isSink(states[i])) return true;
	}
	return false;
}

/** @brief премахва недостижимите състояния */
void FiniteAutomata::removeSinkNodes()
{
	while (areThereSinks()) {
		for (size_t i = 0; i < states.size(); i++)
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
				delete temp;
				--i;
			}
		}
	}
}

/** @brief прави автомат за езика на даден символ  */
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

	//добавям преходи
	q1->nextState.push_back(std::vector<State*>());
	q2->nextState.push_back(std::vector<State*>());
	q1->nextState[0].push_back(q2);

	return result;
}

/** @brief връща следващия елемент от даден низ на регулярен израз от дадена позиция в низа  */
std::string FiniteAutomata::getNextAutomata(std::string str, int& curInd)
{
	std::string result;

	if (!isOperation(str[curInd]) && str[curInd] != ')') {
		result.push_back(str[curInd]);
		--curInd; 
		return result;
	}
	else if (str[curInd] == ')') {
		int count = 1;
		for (int i = curInd - 1; i >= 0; i--) {
			if (str[i] == ')') ++count;
			else if (str[i] == '(') --count;
			if (count == 0) {
				curInd = i - 1;
				std::reverse(result.begin(), result.end());
				return result;
			}
			result.push_back(str[i]);
		}
	}
	else if (str[curInd] == '*') {
		--curInd;
		//добавям скоби, защото ако имам (a+b)* ще стане a+b*, което е грешно
		result.push_back('(');
		std::string next = getNextAutomata(str, curInd);
		result.insert(result.end(), next.begin(), next.end());
		result.push_back(')');
		result.push_back('*');
		return result;
	}

	return result;
}

/** @brief проверява дали даден символ е операция  */
bool FiniteAutomata::isOperation(char ch) const
{
	switch (ch)
	{
	case '*': return true; break;
	case '+': return true; break;
	case '&': return true; break;
	case '.': return true; break;
	default: return false;
	}
}

/** @brief освобождава динамичната памет на автомата и изпразва масивите  */
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

/** @brief копира информацията на масивите на даден автомат в текущия  */
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

/** @brief връща масив от комбинациите(без повторения) на състоянията, като всяка комбинация е отделен масив  */
std::vector<std::vector<State*>> FiniteAutomata::getStateCombinations(std::vector<State*> arr, int k)
{
	// k е броят на състоянията за всяка комбинация, т.е ако k==2 комбинациите на
	// a,b,c ще са от по 2 елемента като ab, ac, bc
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

/** @brief проверява дали дадена дума принадлежи на езика на автомата  */
bool FiniteAutomata::containsWord(const std::string& str) const
{
	if (!areAllCharactersFromAlphabet(str) && !isRandSymbolInAlphabet()) return false;
	int size = start.size();
	for (size_t i = 0; i < size; i++)
	{
		if (containsWordFrom(start[i], str)) return true;
	}
	return false;
}

/** @brief проверавя дали автоматът е детерминиран  */
bool FiniteAutomata::isDeterministic() const
{
	for (size_t i = 0; i < states.size(); i++)
	{
		for (size_t j = 0; j < states[i]->nextState.size(); j++)
		{
			//ако автоматът е детерминиран, всяко състояние ще има точно един преход с всяка буква
			if (states[i]->nextState[j].size() != 1 ) return false;
		}
	}
	//детерминираните автомати имат само едно начално състояние и не работят с епсилон преходи
	if (start.size() != 1 || emptyStrTransition)return false;

	return true;
}

/** @brief принтира информацията на автомата  */
void FiniteAutomata::getInfo() const
{
	int sSize = states.size();
	std::cout << "> Information\n";
	std::cout << "- The automata is ";
	if (isDeterministic()) std::cout << "deterministic with "<< sSize<<" states.\n";
	else {
		std::cout << "nondeterministic with " << sSize << " states.\n";
		if (start.size() == 1)
			std::cout << "- There is " << 1 << " starting state.\n";
		else std::cout << "- There are " << start.size() << " starting states.\n";
	}

	if(acceptingStates.size() == 1)
		std::cout << "- There is " << 1 << " accepting state.\n";
	else std::cout << "- There are " << acceptingStates.size() << " accepting states.\n";
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
		std::string set = "{"; 
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
	
}

/** @brief предефиниция на оператор>> за автомати*/
std::istream& operator>>(std::istream& in, FiniteAutomata& a)
{
	int statesSize, size, alpSize;
	in >> statesSize;
	for (size_t i = 0; i < statesSize; i++)
	{
		a.states.push_back(new State());
	}
	in >> size;
	in.ignore(2, '\n');
	in >> alpSize;
	char ch;
	for (size_t i = 0; i < alpSize; i++)
	{
		in >> ch;
		a.alphabet.push_back(ch);
		for (size_t j = 0; j < statesSize; j++)
		{
			a.states[j]->nextState.push_back(std::vector<State*>());
		}
	}

	in.ignore(1, '\n');
	in.ignore(1, '\n');
	
	int num1, num2;
	for (size_t i = 0; i < size; i++)
	{
		in >> num1 >> ch >> num2;
		int alpInd = a.getCharIndFromAlphabet(ch);
		int q1Ind = num1 - 1;
		int q2Ind = num2 - 1;
		a.states[q1Ind]->nextState[alpInd].push_back(a.states[q2Ind]);
	}
	in.ignore(1, '\n');
	int startSize, accSize;
	in >> startSize >> accSize;
	for (size_t i = 0; i < startSize; i++)
	{
		in >> num1;
		a.start.push_back(a.states[num1 - 1]);
	}
	for (size_t i = 0; i < accSize; i++)
	{
		in >> num1;
		a.acceptingStates.push_back(a.states[num1 - 1]);
		a.states[num1 - 1]->accepting = true;
	}
	return in;
}

/** @brief предефиниция на оператор<< за автомати*/
std::ostream& operator<<(std::ostream& out, const FiniteAutomata& a)
{
	out << a.states.size() << " " << a.getCountOfTransitions() << "\n";
	out << a.alphabet.size() << " ";
	for (size_t i = 0; i < a.alphabet.size(); i++)
	{
		out << a.alphabet[i] << " ";
	}
	out <<"\n\n";
	int q1, q2;
	for (size_t i = 0; i < a.states.size(); i++)
	{
		q1 = i;
		for (size_t j = 0; j < a.states[i]->nextState.size(); j++)
		{
			for (size_t k = 0; k < a.states[i]->nextState[j].size(); k++)
			{
				q2 = a.getStateIndFromAllStates(*a.states[i]->nextState[j][k]);
				out << (q1+1) << " " << a.alphabet[j] << " " << (q2+1) << "\n";
			}
		}
	}

	out << "\n" << a.start.size() << " " << a.acceptingStates.size() << "\n";
	for (size_t i = 0; i < a.start.size(); i++)
	{
		q1 = a.getStateIndFromAllStates(*a.start[i]) + 1;

		out << q1 << " ";
	}

	out << "\n";
	for (size_t i = 0; i < a.acceptingStates.size(); i++)
	{
		q1 = a.getStateIndFromAllStates(*a.acceptingStates[i]) + 1;

		out << q1 << " ";
	}
	return out;
}

#endif