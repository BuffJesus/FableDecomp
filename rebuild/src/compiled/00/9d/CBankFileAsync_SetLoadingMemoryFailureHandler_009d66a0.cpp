#include "fable_game_text_bank.h"
#include "fable_loading_memory_failure_handler.h"

template <typename T> void CCountedPointer<T>::ReleaseData()
{
    if (Info)
    {
        --Info->RefCount;
        if (Info->RefCount == 0)
        {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
template <typename T> void CCountedPointer<T>::ShareData(T* data, CCPPointerInfo* info)
{
    if (Info != info)
    {
        ReleaseData();
        Data = data;
        Info = info;
        if (info) ++info->RefCount;
    }
}
template <typename T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}

// Retail 009D66A0: move this bank's registration to the new handler.
void CBankFileAsync::SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler)
{
    if (MemoryFailureHandler.Get())
        MemoryFailureHandler->UnregisterBank(this);
    if (handler.Get())
        handler->RegisterBank(this);
    MemoryFailureHandler = handler;
}
