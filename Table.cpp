#include "Table.h"
#include "Utils.h"
#include <iostream>
#include <cassert>
using namespace string_utils;

/** @brief изтрива динамичната памет */
void Table::del()
{
	for (size_t i = 0; i < size; ++i)
	{
		delete cols[i];
	}
	delete[] cols;
}

/** @brief добавя още 1 място за колона таблицата */
void Table::addSpace()
{
	IColumn* temp[240];
	for (size_t i = 0; i < size; ++i)
	{
		if (cols[i]->getType() == "int") {
			temp[i] = new Column<int>("int");
		}
		else if (cols[i]->getType() == "string") {
			temp[i] = new Column<std::string>("string");
		}
		else if (cols[i]->getType() == "double") {
			temp[i] = new Column<double>("double");
		}

		*temp[i] = *cols[i];
	}

	del();

	cols = new IColumn * [++size];
	for (size_t i = 0; i < size - 1; ++i)
	{
		if (temp[i]->getType() == "int") {
			cols[i] = new Column<int>("int");
		}
		else if (temp[i]->getType() == "string") {
			cols[i] = new Column<std::string>("string");
		}
		else if (temp[i]->getType() == "double") {
			cols[i] = new Column<double>("double");
		}

		*cols[i] = *temp[i];
	}

	for (size_t i = 0; i < size - 1; ++i)
	{
		delete temp[i];
	}
}

/** @brief конструктор по подразбиране */
Table::Table() : cols(nullptr), size(0), name("")
{
}

/** @brief копи конструктор */
Table::Table(const Table& other) : Table(other.getCols(), other.getSize(), other.getName(), other.getFileName())
{
}

/** @brief конструктор с параметри */
Table::Table(IColumn** arr, size_t s, const std::string& n) : name(n), cols(nullptr), size(0)
{
	setCol(arr, s);
}

/** @brief конструктор с параметри */
Table::Table(IColumn** arr, size_t s, const std::string& n, const std::string& fname) : Table(arr, s, n)
{
	setFileName(fname);
}

/** @brief деструктор */
Table::~Table()
{
	del();
}

/** @brief принтира таблицата */
std::ostream& Table::print(std::ostream& out) const
{
	printTableLine(out);

	printTableTitles(out);

	printTableRow(out);

	printTableLine(out);

	return out;
}

/** @brief принтира линията на таблицата */
std::ostream& Table::printTableLine(std::ostream& out) const
{
	for (size_t i = 0; i < size; ++i)
	{
		cols[i]->printCellLine(out);
	}
	out << "+\n";
	return out;
}

/** @brief принтира заглавията */
std::ostream& Table::printTableTitles(std::ostream& out) const
{
	for (size_t i = 0; i < size; ++i)
	{
		cols[i]->printCellTitle(out);
	}

	out << "|\n";
	return out;
}

/** @brief принтира редовете */
std::ostream& Table::printTableRow(std::ostream& out) const
{
	size_t max = getMaxRowSize();

	for (size_t i = 0; i < max; ++i)
	{
		printTableLine(out);
		for (size_t j = 0; j < size; ++j)
		{
			if (cols[j]->getRowSize() < i + 1)	cols[j]->printCellContents(-1, out);
			else cols[j]->printCellContents(i, out);
		}
		out << "|\n";
	}
	return out;
}

/** @brief прочита таблица от файл или стандартния вход */
std::istream& Table::read(std::istream& in)
{
	in.ignore(12);
	getline(in, name);

	in.ignore(9);
	in >> size;

	size_t rows;
	in.ignore(6);
	in >> rows;

	cols = new IColumn * [size];

	in.ignore(1, '\n');
	in.ignore(14);

	char* temp = new char[100];
	for (size_t i = 0; i < size; ++i)
	{
		if (i == size - 1) in.getline(temp, 64, ' ');
		else in.getline(temp, 64, ',');
		in.ignore(1);
		if (strcmp(temp, "int") == 0) {
			cols[i] = new Column<int>("int");
		}
		else if (strcmp(temp, "string") == 0) {
			cols[i] = new Column<std::string>("string");
		}
		else if (strcmp(temp, "double") == 0) {
			cols[i] = new Column<double>("double");
		}
		//cols[i]->setType(temp);
	}
	in.ignore(2, '\n\n');
	in.ignore(256, '\n');

	for (size_t i = 0; i < size; ++i)
	{
		in.ignore(1);
		in.getline(temp, 64, '|');
		cols[i]->setTitle(removePaddingFromStr(temp));
	}
	in.ignore(1);

	for (size_t i = 0; i < rows - 1; ++i)
	{
		in.ignore(256, '\n'); //игнорира линията
		for (size_t j = 0; j < size; ++j)
		{
			in.ignore(1);
			in.getline(temp, 64, '|');
			cols[j]->addCell(removePaddingFromStr(temp)); //премахва празното място около съдържанието на клетката и я добавя
		}
		in.ignore(2, '\n');
	}

	delete[] temp;
	return in;
}

/** @brief записва таблица във файл или извежда на екрана чрез стандартния изход */
std::ostream& Table::write(std::ostream& out) const
{
	out << "Table name: " << name << std::endl
		<< "Columns: " << size << std::endl
		<< "Rows: " << getMaxRowSize() + 1 << std::endl
		<< "Column types: ";

	for (size_t i = 0; i < size; ++i)
	{
		out << cols[i]->getType();
		if (i == size - 1)
			out << " ";
		else out << ", ";

	}
	out << std::endl << std::endl;
	print(out);

	return out;
}

/** @brief връща копие на подадената дума без допълнителното място около нея */
std::string Table::removePaddingFromStr(std::string s)
{
	std::string result = s;
	size_t ind = 0;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] != ' ') {
			result[ind++] = s[i];
		}
		else if (isSpaceBetweenWords(s, i)) {
			result[ind++] = s[i];
		}
	}
	result.resize(ind);

	return result;
}

/** @brief принтира редовете на таблица от един индекс до друг */
void Table::printFromTo(size_t start, size_t end) const
{

	printTableLine(std::cout);

	printTableTitles(std::cout);

	printRows(start, end);

	printTableLine(std::cout);

}

/** @brief принтира редовете на таблица */
void Table::printRows(size_t start, size_t end) const
{
	for (size_t i = start; i < end; ++i)
	{
		printTableLine(std::cout);
		for (size_t j = 0; j < size; ++j)
		{
			if (cols[j]->getRowSize() < i + 1)	cols[j]->printCellContents(-1, std::cout);
			else cols[j]->printCellContents(i, std::cout);
		}
		std::cout << "|\n";
	}
}

/** @brief принтира реда на даден индекс */
void Table::printRowAt(size_t ind) const
{
	printTableLine(std::cout);
	for (size_t j = 0; j < size; ++j)
	{
		cols[j]->printCellContents(ind, std::cout);
	}
	std::cout << "|\n";

}

/** @brief задава стойности на колоните */
void Table::setCol(IColumn* arr[], size_t s)
{
	for (size_t i = 0; i < size; ++i)
	{
		delete cols[i];
	}
	delete[] cols;

	size = s;
	cols = new IColumn * [s];

	for (size_t i = 0; i < size; ++i)
	{
		int j = 0;
		if (arr[i]->getType() == "string") {
			cols[i] = new Column<std::string>;
		}
		else if (arr[i]->getType() == "double") {
			cols[i] = new Column<double>;
		}
		else {
			cols[i] = new Column<int>;
		}
		cols[i] = arr[i];
	}

}

/** @brief задава нова стойност на името на файла */
void Table::setFileName(const std::string& str)
{
	filename = str;
}

/** @brief задава нов име */
void Table::setName(const std::string& newName)
{
	name = newName;
}

/** @brief добавя нова колона */
void Table::addCol(std::string colName, std::string type)
{
	addSpace();

	--size;
	if (type == "int") {
		cols[size] = new Column<int>("int", colName, getMaxRowSize());
	}
	else if (type == "string") {
		cols[size] = new Column<std::string>("string", colName, getMaxRowSize());
	}
	else if (type == "double") {
		cols[size] = new Column<double>("double", colName, getMaxRowSize());
	}
	++size;
}

/** @brief връща големината на най-голямата колона */
size_t Table::getMaxRowSize() const
{
	size_t max = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (max < cols[i]->getRowSize()) max = cols[i]->getRowSize();
	}
	size_t colSize = max;
	return max;
}

/** @brief връща броя на колоните */
size_t Table::getSize() const
{
	return size;
}

/** @brief връща името на файла, в който е записан */
std::string Table::getFileName() const
{
	return filename;
}

/** @brief връща името на таблицата */
std::string Table::getName() const
{
	return name;
}

/** @brief връща масива от указатели към колони на таблицата */
IColumn** Table::getCols() const
{
	return cols;
}

/** @brief връща указател към колона с дадено име */
IColumn* Table::getColumnByName(std::string name) const
{
	for (size_t i = 0; i < size; ++i)
	{
		if (cols[i]->getTitle() == name) return cols[i];
	}
	return nullptr;
}
