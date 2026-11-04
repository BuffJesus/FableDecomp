#include "engine/CEngineStaticMapFileBlockManager.h"

template<> inline CCountedPointer<CBankFileAsyncEntry>&
CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer& other)
{
    CBankFileAsyncEntry* data=other.Data;
    CCPPointerInfo* info=other.Info;
    if(Info!=info) {
        Release();
        Data=data;
        Info=info;
        if(Info) ++Info->RefCount;
    }
    return *this;
}

void CEngineStaticMapFileBlockManager::SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry> pool)
{
    AsyncBankEntry=pool;
}
