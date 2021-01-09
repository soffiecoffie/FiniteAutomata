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

/** @brief премахва всички празни места на даден низ */
std::string removeSpaces(std::string str) {
	return removeChar(str, ' ');
}

/** @brief премахва всички срещания на даден символ от даден низ */
std::string removeChar(std::string str, char ch)
{
	std::string result;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] != ch) {
			result.push_back(str[i]);
		}
	}
	return result;
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

int factorial(int n)
{
	if (n == 0) return 1;

	return n * factorial(n - 1);
}

int combination(int n, int k)
{
	return factorial(n)/( factorial(k) * factorial(n-k) );
}

std::vector<std::vector<int>> comb(std::vector<int> arr, int k)
{
	std::vector<std::vector<int>> result;
	if (k == 0) return result;
	if (k == 1) {
		for (size_t i = 0; i < arr.size(); i++)
		{
			std::vector<int> v;
			v.push_back(arr[i]);
			result.push_back(v);
		}
		return result;
	}

	int size = arr.size();
	for (size_t i = 0; i < size; i++)
	{
		std::vector<int> v;
		std::vector<int> temp;
		v.push_back(arr[i]);
		temp = v;
		for (size_t j = i + 1; j < size; j++) 
		{
			v.push_back(arr[j]);
			if (k > 2) {
				std::vector<int> temp2;
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

	std::cout << "\nCombs:\n";
	for (size_t i = 0; i < result.size(); i++)
	{
		for (size_t j = 0; j < result[i].size(); j++)
		{
			std::cout << result[i][j];
		}
		std::cout << " ";
	}
	std::cout << "\n";

	return result;
}
