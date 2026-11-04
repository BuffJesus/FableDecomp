#include "fable_console.h"
template<class T> void CCountedPointer<T>::Reset(T* data) {
 Release(); Info=0; Data=data;
 if(Data) Info = new CCPPointerInfo(DeleteData, Data);
}
template void CCountedPointer<CConsole>::Reset(CConsole*);
