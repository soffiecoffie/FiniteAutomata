#include "Utils.h"
#include <iostream>
#include <cassert>

const double eps = 0.9999999999;

/** @brief връща броя на цифрите на дадено число */
int digits(int n)
{
	if (n == 0)	return 0;
	return 1 + digits(n / 10);
}

/** @brief връща броя на цифрите преди десетичната запетайка на дадено число */
int digits(double n)
{
	if (fabs(n - 0) < eps)	return 0;
	return 1 + digits(n / 10);
}

/** @brief връща истина ако подаденото число е нечетно */
bool isOdd(int n)
{
	return n % 2 != 0;
}

/** @brief извежда на екрана даден символ по даден брой пъти */
std::ostream& printMany(const char* ch, size_t n, std::ostream& out)
{
	for (size_t i = 0; i < n; ++i)
	{
		out << ch;
	}
	return out;
}


/** @brief проверява дали празното място на подадения индекс в дадения низ се намира между думи */
bool isSpaceBetweenWords(std::string str, size_t ind)
{
	assert(str[ind] == ' ');
	if (ind != str.size() - 1 && ind != 0) {
		return (str[ind - 1] != ' ') && (str[ind + 1] != ' ');
	}
	return false; //ако е първия или последния символ 
}

/** @brief връща думата, която е n-та по ред в подадения низ str */
std::string getWord(size_t num, const std::string& str)
{
	assert(numOfWords(str) >= num);
	std::string result = str;
	size_t start = 0;
	size_t counter = 0;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ') {
			if (num == 1) {
				result.erase(result.begin() + i, result.end());
				return result;
			}
			++counter;
			if (counter == (num - 1)) start = i;
			else if (counter == num) {
				result.erase(result.begin() + i, result.end());
				result.erase(0, start + 1);

				return result;
			}
		}

		if (i == str.size() - 1) {
			result.erase(0, start + 1);
			return result;
		}
	}
}

/** @brief връща броя на думите в даден низ */
size_t numOfWords(std::string s)
{
	size_t counter = 0;
	for (size_t i = 1; i < s.size() - 1; ++i)	 //така няма да брои празното място ако е в началото или края
	{
		if (s[i] == ' ') ++counter;
	}
	return counter + 1;
}

/** @brief проверява дали даден низ се намира в друг */
bool hasStr(const std::string& text, const std::string& str)
{
	int count = 0;
	int ind;
	size_t i = 0;
	if (str.size() > text.size()) return false;

	for (size_t j = 0; j < str.size(); )
	{
		if (i >= text.size()) break;
		if (count == 0) ind = i;
		if (text[i] == str[j]) {
			++count;
			if (count == str.size()) return true;
			if (count == 1) ind = i;
			++i;
			++j;
		}
		else {
			count = 0;
			i = ++ind;
			j = 0;
		}
	}
	return false;
}