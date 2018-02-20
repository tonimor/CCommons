#ifndef QSORTOBJECT_H
#define QSORTOBJECT_H

#include <string>
#include <vector>
using namespace std;

#include "CCommons_dfdllh1.h"

typedef vector<int> IntArray;
typedef vector<std::string> StringArray;
typedef vector<unsigned long int> DWordArray;

class QSortObject
{
public:
    QSortObject();

    virtual void QSort() = 0;
    virtual void QSort(IntArray& i_arrayToSort);
    virtual void QSort(StringArray& i_arrayToSort);
    virtual int  QSortCompareFunction(const void *, const void *) = 0;

protected:
     StringArray m_stringBuffer;
     IntArray    m_intBuffer;
};

static QSortObject* pThis = NULL;

#include "CCommons_dfdllh2.h"

#endif // QSORTOBJECT_H
