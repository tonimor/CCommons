#include "qsortint.h"

QSortInt::QSortInt()
{
    m_arrayToSort = NULL;
}

QSortInt::QSortInt(IntArray* i_arrayToSort)
{
    m_arrayToSort = i_arrayToSort;
}

void QSortInt::QSort(IntArray& i_arrayToSort)
{
    pThis = this;
    QSortObject::QSort(i_arrayToSort);
}

void QSortInt::QSort()
{
    pThis = this;
    QSortObject::QSort(*m_arrayToSort);
}

int QSortInt::QSortCompareFunction(const void *i_arg1, const void *i_arg2)
{
    int	value1 = m_intBuffer[*(unsigned long *)i_arg1];
    int	value2 = m_intBuffer[*(unsigned long *)i_arg2];

    if(value1 > value2)
        return 1;
    else if(value1 < value2)
        return -1;
    return 0;
}


