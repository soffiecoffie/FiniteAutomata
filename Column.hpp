#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <iomanip>
#include <iostream>
#include <cassert>
#include "Column.h"
#include "Utils.h"

/** @brief конструктор с параметри */
template<typename T>
Column<T>::Column(const std::string& _title, const std::string& _type, const T* _cells, size_t _size)
	: size(0), cells(nullptr), title(_title)
{
	setType(_type);
	setCells(_cells, _size);
}

/** @brief конструктор с параметри */
template<typename T>
Column<T>::Column(const std::string& _title, const std::string& _type, const std::vector<T> _cells, size_t _size)
	: size(0), cells(nullptr), title(_title)
{
	setType(_type);
	setCells(_cells, _size);
}

/** @brief конструктор с параметър */
template<typename T>
Column<T>::Column(const char* type) : size(0), title(""), cells(nullptr)
{
	setType(type);
}

/** @brief конструктор с параметри */
template<typename T>
Column<T>::Column(const std::string& _type, const std::string& _title, size_t s) : size(s), title(_title)
{
	cells = new T * [size];
	for (size_t i = 0; i < size; ++i)
	{
		cells[i] = nullptr;
	}
	setType(_type);
}

/** @brief конструктор по подразбиране */
template<typename T>
Column<T>::Column() : size(0), title(""), type(""), cells(nullptr)
{
}

/** @brief деструктор */
template<typename T>
Column<T>::~Column()
{
	del();
}

/** @brief изтрива динамичната памет */
template<typename T>
void Column<T>::del()
{
	for (size_t i = 0; i < size; ++i)
	{
		delete cells[i];
	}
	delete[] cells;
}

/** @brief добавя още едно място към масива */
template<typename T>
void Column<T>::addSpace()
{
	T* temp[240];
	for (size_t i = 0; i < size; ++i)
	{
		temp[i] = new T;
		if (cells[i] == nullptr) temp[i] = nullptr;
		else *temp[i] = *cells[i];
	}

	del();

	cells = new T * [++size];
	for (size_t i = 0; i < size - 1; ++i)
	{
		if (temp[i] == nullptr) {
			cells[i] = nullptr;
		}
		else {
			cells[i] = new T(*temp[i]);
		}
	}

	for (size_t i = 0; i < size - 1; ++i)
	{
		delete temp[i];
	}
}

/** @brief връща броя на цифрите в клетката */
template<typename T>
size_t Column<T>::getCellLength(size_t ind) const
{
	if (cells[ind] == nullptr) return 4;
	int digitsCount = digits(*cells[ind]);
	return (*cells[ind] < 0) ? ((digitsCount == 0) ? 2 : digitsCount + 1) : ((digitsCount == 0) ? 1 : digitsCount);
	// първото брои знака за минус ако е отрицателно
	// 1ницата във второто е за да може нулата да се принтира без проблеми
}

/** @brief връща броя на цифрите в клетката +1 за десетичната запетая */
template<>
inline size_t Column<double>::getCellLength(size_t ind) const
{
	if (cells[ind] == nullptr) return 4;
	int digitsCount = digits(*cells[ind]);
	//return (digitsCount == 0) ?  1 + 3 : digitsCount + 3;
	return (*cells[ind] < 0) ? ((digitsCount == 0) ? 1 + 4 : digitsCount + 4) : ((digitsCount == 0) ? 1 + 3 : digitsCount + 3);
	// 1 + 4 -> за да преброя минуса
	// digitsCount + 3 -> за да преброя десетичната запетая и 2 цифри след нея 
	// 1 + 3 -> за да няма проблеми с цифри като 0,01
}

/** @brief връща броя на символите в клетката */
template<>
inline size_t Column<std::string>::getCellLength(size_t ind) const
{
	if (cells[ind] == nullptr) return 4;
	return cells[ind]->size();
}

/** @brief принтира линията на клетката */
template<typename T>
std::ostream& Column<T>::printCellLine(std::ostream& out) const
{
	size_t length = getLongest() + 6;
	out << "+";
	printMany("-", length, out);
	return out;
}

/** @brief принтира заглавието на клетката */
template<typename T>
std::ostream& Column<T>::printCellTitle(std::ostream& out) const
{
	size_t length = getLongest() + 6;
	size_t pad = length - title.size();
	out << "|";

	if (isOdd(pad)) printPadding(pad + 1, out);
	else printPadding(pad, out);

	out << title;

	printPadding(pad, out);
	return out;
}

/** @brief принтира съдържанието на клетката */
template<typename T>
std::ostream& Column<T>::printCellContents(size_t i, std::ostream& out) const
{
	size_t pad, length = getLongest() + 6;		//+6 е за празното място около съдържанието на клетката (мин. 3 " " от двете страни)

	if (cells[i] == nullptr || i == -1) {
		pad = length - 4;
		out << "|";

		if (isOdd(pad)) printPadding(pad + 1, out);
		else printPadding(pad, out);

		out << "NULL";
	}
	else {
		pad = length - getCellLength(i);

		out << "|";

		if (isOdd(pad)) printPadding(pad + 1, out);
		else printPadding(pad, out);

		if (type == "double") out << std::fixed << std::setprecision(2) << *cells[i];
		else out << *cells[i];
	}

	printPadding(pad, out);

	return out;
}

/** @brief принтира празно място */
template<typename T>
std::ostream& Column<T>::printPadding(size_t pad, std::ostream& out) const
{
	printMany(" ", pad / 2, out);
	return out;
}

/** @brief връща големината на колоната */
template<typename T>
size_t Column<T>::getRowSize() const
{
	return size;
}

/** @brief връща типа на колоната като низ */
template<typename T>
std::string Column<T>::getType() const
{
	return type;
}

/** @brief връща заглавието на колоната */
template<typename T>
std::string Column<T>::getTitle() const
{
	return title;
}

/** @brief връща стойността на дадена клетка като низ */
template<typename T>
std::string Column<T>::getCellAt(size_t ind) const
{
	if (cells[ind] == nullptr) return "NULL";
	return std::to_string(*cells[ind]);
}

/** @brief връща низ от стойността на дадена клетка на колона от тип std::string */
template<>
inline std::string Column<std::string>::getCellAt(size_t ind) const
{
	return *cells[ind];
}

/** @brief връща масив от указатели към стойностите на колоната */
template<typename T>
T** Column<T>::getCells() const
{
	return cells;
}

/** @brief заменя стойностите на клетките на текущата колона с тези на друга */
template<typename T>
void Column<T>::copyOtherCells(const IColumn& other)
{
	del();

	size = other.getRowSize();

	cells = new T * [size];
	for (size_t i = 0; i < size; ++i)
	{
		if (other.getCellAt(i) == "NULL") cells[i] = nullptr;
		else cells[i] = new T(other.getCellAt(i));
	}
}

/** @brief заменя стойностите на клетките на текущата колона с тези на друга */
template<>
inline void Column<int>::copyOtherCells(const IColumn& other)
{
	del();

	size = other.getRowSize();

	cells = new int* [size];
	for (size_t i = 0; i < size; ++i)
	{
		if (other.getCellAt(i) == "NULL") cells[i] = nullptr;
		else cells[i] = new int(std::stoi(other.getCellAt(i)));
	}
}

/** @brief заменя стойностите на клетките на текущата колона с тези на друга */
template<>
inline void Column<double>::copyOtherCells(const IColumn& other)
{
	del();

	size = other.getRowSize();

	cells = new double* [size];
	for (size_t i = 0; i < size; ++i)
	{
		if (other.getCellAt(i) == "NULL") cells[i] = nullptr;
		else cells[i] = new double(std::stod(other.getCellAt(i)));
	}
}

/** @brief добавя нова клетка */
template<typename T>
void Column<T>::addCell(std::string cell)
{
	addSpace();

	if (cell == "NULL") cells[size - 1] = nullptr;
	else cells[size - 1] = new T(cell);
}

/** @brief добавя нова клетка в колона от тип int */
template<>
inline void Column<int>::addCell(std::string cell)
{
	addSpace();

	if (cell == "NULL") cells[size - 1] = nullptr;
	else cells[size - 1] = new int(std::stoi(cell));
}

/** @brief добавя нова клетка в колона от тип double */
template<>
inline void Column<double>::addCell(std::string cell)
{
	addSpace();

	if (cell == "NULL") cells[size - 1] = nullptr;
	else cells[size - 1] = new double(std::stod(cell));
}

/** @brief задава нова стойност на заглавието */
template<typename T>
void Column<T>::setTitle(const std::string& t)
{
	title = t;
}

/** @brief задава нови стойности на клетките на колоната */
template<typename T>
void Column<T>::setCells(const T* arr, size_t s)
{
	del();

	size = s;

	cells = new T * [s];
	for (size_t i = 0; i < s; ++i)
	{
		cells[i] = new T(arr[i]);
	}
}

/** @brief задава нови стойности на клетките на колоната */
template<typename T>
void Column<T>::setCells(const std::vector<T> arr, size_t s)
{
	del();

	size = s;

	cells = new T * [s];
	for (size_t i = 0; i < s; ++i)
	{
		cells[i] = new T(arr[i]);
	}
}
/** @brief задава стойност на типа на колоната */
template<typename T>
void Column<T>::setType(const std::string& t)
{
	assert(t == "int" || t == "string" || t == "double");
	type = t;
}

/** @brief връща дължината на най-дългата клетка */
template<typename T>
size_t Column<T>::getLongest() const
{
	int max = 0;
	for (size_t i = 1; i < size; ++i)
	{
		if (getCellLength(max) < getCellLength(i)) {
			max = i;
		}
	}
	if (title.size() > getCellLength(max)) {
		return title.size();
	}
	return getCellLength(max);
}

/** @brief принтира колоната */
template<typename T>
void Column<T>::print() const
{
	printCellLine(std::cout);
	std::cout << "+\n";
	printCellTitle(std::cout);
	std::cout << "|\n";

	printCellLine(std::cout);
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << "+\n";
		printCellContents(i, std::cout);
		std::cout << "|\n";
		printCellLine(std::cout);
	}
	std::cout << "+";
	std::cout << "\n";
}

/** @brief задава стойността на дадена клетка да бъде nullptr */
template<typename T>
void Column<T>::setNullCellAt(size_t ind)
{
	cells[ind] = nullptr;
}

#endif