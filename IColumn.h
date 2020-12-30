#ifndef ICOLUMN_H
#define ICOLUMN_H

#include <string>

/** @brief клас IColumn е интерфейсен клас на шаблонния клас Column<T> */
class IColumn
{

public:
	/** @brief деструктор */
	virtual ~IColumn() = 0 {};
	virtual void addCell(std::string) = 0;

	virtual std::ostream& printCellLine(std::ostream&) const = 0;
	virtual std::ostream& printCellTitle(std::ostream&) const = 0;
	virtual std::ostream& printCellContents(size_t, std::ostream&) const = 0;

	virtual size_t getRowSize() const = 0;
	virtual std::string getType() const = 0;
	virtual std::string getTitle() const = 0;

	virtual std::string getCellAt(size_t) const = 0;

	virtual void setNullCellAt(size_t) = 0;

	virtual void setType(const std::string&) = 0;
	virtual void setTitle(const std::string&) = 0;
	virtual void copyOtherCells(const IColumn&) = 0;
	virtual void print() const = 0;

	IColumn& operator=(const IColumn&);
};

#endif
