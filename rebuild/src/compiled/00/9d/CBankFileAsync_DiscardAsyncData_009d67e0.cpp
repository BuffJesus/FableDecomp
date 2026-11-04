#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
template<class T> inline void CCountedPointer<T>::Reset(T* data) {
    Release();
    Info=0;
    Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
static __forceinline bool FinishedForDiscard(const CBankFileAsyncData& data) {
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(data.BankFile.CriticalSection),false);
    if(data.Finished) {
        data.AllowRestart=false;
        return true;
    }
    return false;
}
void CBankFileAsync::DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data) {
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection),false);
    if(!data.IsNull()) {
        if(!FinishedForDiscard(*data)&&!data->Abort())DisposalList.push_back(data);
        data.Reset(0);
    }
    ProcessDisposalList();
}
