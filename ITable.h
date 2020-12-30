#ifndef ITable_H
#define ITable_H

#include "Column.h"
#include <string>

/** @brief клас ITable е интерфейсен клас на Table */
class ITable
{
public:
	/** @brief деструктор */
	virtual ~ITable() = 0 {};

	virtual std::ostream& print(std::ostream& out) const = 0;

	virtual std::istream& read(std::istream&) = 0;
	virtual std::ostream& write(std::ostream&) const = 0;

	virtual size_t getSize() const = 0;
	virtual std::string getFileName() const = 0;
	virtual std::string getName() const = 0;
	virtual IColumn** getCols() const = 0;
	virtual IColumn* getColumnByName(std::string) const = 0;
	virtual size_t getMaxRowSize() const = 0;

	virtual void addCol(std::string, std::string) = 0;

	virtual void setFileName(const std::string&) = 0;
	virtual void setName(const std::string&) = 0;


	virtual void printFromTo(size_t, size_t) const = 0;
	virtual void printRows(size_t, size_t) const = 0;
	virtual void printRowAt(size_t) const = 0;
};

#endif