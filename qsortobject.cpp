#include "qsortobject.h"
#include "assert.h"

extern "C" {
    int qsort_compare_function(const void *arg1, const void *arg2) {
        return pThis->QSortCompareFunction(arg1, arg2);
    }
}

QSortObject::QSortObject()
{
}

void QSortObject::QSort(StringArray& i_arrayToSort)
{
    size_t size = i_arrayToSort.size();
    if(size == 0)
        return;

    assert(m_stringBuffer.size() == 0);
	if(size == 0)
		return;

    m_stringBuffer = i_arrayToSort;

    // create the array to sort
    DWordArray array(size);

    // Set the current order
    for(unsigned int iIndex = 0; iIndex < size; iIndex++) {
        array[iIndex] = iIndex;
    }

    // Order the indexes
    pThis = this;
	qsort(&array[0], array.size(), sizeof(unsigned long int), qsort_compare_function);

    // Modify the order by means the indexes
    for(iIndex = 0; iIndex < i_arrayToSort.size(); iIndex++)
        i_arrayToSort[iIndex] = m_stringBuffer[array[iIndex]];

    m_stringBuffer.erase(m_stringBuffer.begin(), m_stringBuffer.end());
}

void QSortObject::QSort(IntArray& i_arrayToSort)
{
    size_t size = i_arrayToSort.size();
    if(size == 0)
        return;

    assert(m_intBuffer.size() == 0);
	if(size == 0)
		return;

    m_intBuffer = i_arrayToSort;

    // create the array to sort
    DWordArray array(size);

    // Set the current order
    for(unsigned int iIndex = 0; iIndex < size; iIndex++) {
        array[iIndex] = iIndex;
    }

    // Order the indexes
    pThis = this;
    qsort(&array[0], array.size(), sizeof(unsigned long int), qsort_compare_function);

    // Modify the order by means the indexes
    for(iIndex = 0; iIndex < i_arrayToSort.size(); iIndex++)
        i_arrayToSort[iIndex] = m_intBuffer[array[iIndex]];

    m_intBuffer.erase(m_intBuffer.begin(), m_intBuffer.end());
}
