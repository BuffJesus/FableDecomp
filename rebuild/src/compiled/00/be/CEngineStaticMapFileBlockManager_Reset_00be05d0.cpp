#include "engine/CEngineStaticMapFileBlockManager.h"
#include "fable_stlport_tree_erase.h"
template<class T> inline void CCountedPointer<T>::Reset(T* data) {
    Release();Info=0;Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
void CEngineStaticMapFileBlockManager::Reset() {
    _STL::map<long,CFileBlock>::iterator current=Blocks.begin();
    while(current!=Blocks.end()) {
        _STL::map<long,CFileBlock>::iterator next=current;
        ++next;
        Blocks.erase(current);
        current=next;
    }
    AsyncBankEntry.Reset(0);
}
