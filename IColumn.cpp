#include "IColumn.h"


/** @brief предефинира оператор = */
IColumn& IColumn::operator=(const IColumn& other)
{
	if (this != &other) {
		setTitle(other.getTitle());
		setType(other.getType());
		copyOtherCells(other);
	}
	return *this;
}
