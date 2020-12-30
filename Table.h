#ifndef TABLE_H
#define TABLE_H

#include "ITable.h"
#include "Column.h"
#include <string>

/** @brief клас Table създава таблици */
class Table : public ITable
{
	size_t size;
	IColumn** cols;
	std::string name;
	std::string filename = "";

private:
	void del();
	void addSpace();

public:
	Table();
	Table(const Table&);// = delete;
	Table(IColumn**, size_t, const std::string&);
	Table(IColumn**, size_t, const std::string&, const std::string&);
	~Table();

	std::ostream& print(std::ostream& out) const;
	std::ostream& printTableLine(std::ostream& out) const;
	std::ostream& printTableTitles(std::ostream& out) const;
	std::ostream& printTableRow(std::ostream& out) const;

	std::istream& read(std::istream&);
	std::ostream& write(std::ostream&) const;

	std::string removePaddingFromStr(std::string s);

	void printFromTo(size_t, size_t) const;
	void printRows(size_t, size_t) const;
	void printRowAt(size_t) const;

	void setCol(IColumn* [], size_t);
	void setFileName(const std::string&);
	void setName(const std::string&);
	void addCol(std::string, std::string);

	size_t getMaxRowSize() const;
	size_t getSize() const;
	std::string getFileName() const;
	std::string getName() const;
	IColumn** getCols() const;
	IColumn* getColumnByName(std::string) const;

};

#endif