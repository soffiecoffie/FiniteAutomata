#include "FiniteAutomata.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <string>
using namespace number_utils;
using namespace string_utils;

int main()
{
	std::cout << "Input file name:\n";
	std::string filename;
	std::cin >> filename;			//file.TXT
	std::cout << "Input regex:\n";
	std::string regex;
	std::cin >> regex;				//a*.b.b*+a*
	FiniteAutomata a;

	std::cout << "From " << filename << " with regex "<< regex << " are read these rows:\n";
	a.printAllSentencesFromRegex(filename, regex);

	/*
	FiniteAutomata empty;
	empty = empty.regexToAutomata("a*.b.b*+a*");
	empty.determine();
	empty.getInfo();
	std::cout << "\n\n\nFrom " << "file.TXT" << " with regex \"a*.b.b*+c+@+d*.a\" are read these rows:\n";
	empty.printAllSentencesFromRegex("file.TXT", "a*.b.b*+c+@+d*.a");
	std::cout << "\n\n";


	std::string name = "empty.txt";
	//	std::cin >> name;

	FiniteAutomata empty2;
	std::ifstream in(name);
	if (!in) std::cout << "Can't open file " << name << "!\n";
	in >> empty2;
	empty2.getInfo();
	in.close();
	*/
	
	/*  // Writing to file
	std::ofstream out(name);
	if (!out) std::cout << "Can't open file " << name << "!\n";
	out << empty;
	out.close();
	*/
	return 0;
}