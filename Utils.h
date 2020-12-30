#ifndef UTILS_H
#define UTILS_H
#include <fstream>
#include <string>

//Полезни функции

int digits(int);
int digits(double n);
bool isOdd(int);
std::ostream& printMany(const char*, size_t, std::ostream&);
bool isSpaceBetweenWords(std::string, size_t);
std::string getWord(size_t, const std::string&);
size_t numOfWords(std::string);
bool hasStr(const std::string&, const std::string&);

#endif 