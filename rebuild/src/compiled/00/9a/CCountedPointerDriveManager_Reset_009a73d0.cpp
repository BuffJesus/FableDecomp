#include "fable_system_manager.h"
template<class T> void CCountedPointer<T>::Reset(T* data) {
 Release(); Info=0; Data=data;
 if(Data) Info = new CCPPointerInfo(DeleteData, Data);
}
template void CCountedPointer<CDriveManager>::Reset(CDriveManager*);