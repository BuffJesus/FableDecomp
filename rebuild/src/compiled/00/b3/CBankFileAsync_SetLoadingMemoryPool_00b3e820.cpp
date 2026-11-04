#include "fable_game_text_bank.h"

template<> inline CCountedPointer<CMovableResourceMemoryPool>&
CCountedPointer<CMovableResourceMemoryPool>::operator=(const CCountedPointer& other)
{
    if(Info!=other.Info) {
        Release();
        Data=other.Data;
        Info=other.Info;
        if(Info) ++Info->RefCount;
    }
    return *this;
}

void CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool)
{
    LoadingMemoryPool=pool;
}
