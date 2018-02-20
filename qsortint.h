#ifndef QSORTINT_H
#define QSORTINT_H


/**********************************************************************************
Class to sort int arrays

Usage
-----
IntArray intArray = {5, 1, 4, 1, 4, 2, 9, 7, 6, 8, 3};
QSortInt sortIntObj(&intArray);
sortIntObj.QSort();

************************************************************************************/

#include "qsortobject.h"

#include "CCommons_dfdllh1.h"

class QSortInt : public QSortObject
{
private:
    IntArray* m_arrayToSort;

public:
    QSortInt();
    QSortInt(IntArray* i_arrayToSort);

    virtual void QSort(IntArray& i_arrayToSort);
    virtual void QSort();
    virtual int QSortCompareFunction(const void *i_arg1, const void *);
};

#include "CCommons_dfdllh2.h"

#endif // QSORTINT_H
