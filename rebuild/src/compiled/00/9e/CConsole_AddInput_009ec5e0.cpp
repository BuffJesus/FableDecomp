#include "fable_console.h"
template<class T> inline CCountedPointer<T>::CCountedPointer(T* data) : Data(data) {
 if(Data)Info=new CCPPointerInfo(DeleteData,Data); else Info=0;
}
template<class T> inline CCountedPointer<T>::CCountedPointer(const CCountedPointer& other) : Data(other.Data),Info(other.Info) {
 if(Info)++Info->RefCount;
}
void CConsole::AddInput(CConsoleInputBase* input) {
 RegisteredConsoleInputs.insert(_STL::make_pair(input->GetName().GetAsLowerCase(),CCountedPointer<CConsoleInputBase>(input)));
}