#ifndef QSORTSTRING_H
#define QSORTSTRING_H

/**********************************************************************************
Class to sort string arrays

Usage
-----
StringArray stringArray = {"5", "1", "4", "1", "4", "2", "9", "7", "6", "8", "3"};
QSortString sortStringObj(&stringArray);
sortStringObj.QSort();

************************************************************************************/

#include "qsortobject.h"

#include "CCommons_dfdllh1.h"

class QSortString : public QSortObject
{
private:
    StringArray* m_arrayToSort;

public:
    QSortString();
    QSortString(StringArray* i_arrayToSort);

    virtual void QSort(StringArray& i_arrayToSort);
    virtual void QSort();
    virtual int QSortCompareFunction(const void *i_arg1, const void *);
};

#include "CCommons_dfdllh2.h"

#endif // QSORTSTRING_H
