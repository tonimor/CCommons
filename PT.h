#ifndef PT_H
#define PT_H

/**********************************************************************************
Smart pointer template

Usage
-----
int main()
{
    PT<int> pt_int(new int);
    *pt_int = 10;

    PT<bool> pt_bool(new bool);
    *pt_bool = true;
}

************************************************************************************/

#include "CCommons_dfdllh1.h"

template <typename T>
class PT
{
    T* _ptr;

public:
    explicit PT(T* ptr = 0) : _ptr(ptr) {}
    ~PT() { delete _ptr; }

    // implementamos estos operadores para que la clase actue como puntero
    T& operator*() { return *_ptr; }
    T* operator->() { return _ptr; }
};

#include "CCommons_dfdllh2.h"

#endif // PT_H
