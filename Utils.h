#ifndef UTILS_H
#define UTILS_H
#include <fstream>
#include <string>
#include <vector>

/** @brief namespace, с помощни функции за работа с числа  */
namespace number_utils {
    int digits(int);
    int digits(double n);
    bool isOdd(int);
    int factorial(int);
    int combination(int, int);
    std::vector<std::vector<int>> comb(std::vector<int> arr, int k);
}

/** @brief namespace, с помощни функции за работа с низове  */
namespace string_utils {
    std::ostream& printMany(const char*, size_t, std::ostream&);
    bool isSpaceBetweenWords(std::string, size_t);
    std::string removeSpaces(std::string);
    std::string removeChar(std::string, char);
    std::string getWord(size_t, const std::string&);
    size_t numOfWords(std::string);
    bool hasStr(const std::string&, const std::string&);
}

#endif 