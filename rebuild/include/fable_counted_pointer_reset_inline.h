#pragma once
#include "fable_counted_pointer.h"

template<class T> inline void CCountedPointer<T>::Reset(T* data) {
    Release();
    Info=0;
    Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
