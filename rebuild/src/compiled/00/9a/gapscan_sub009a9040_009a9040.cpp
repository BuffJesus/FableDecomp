#include "fable_threaded_file.h"

// CCountedPointer<CThreadedFile>::DeleteData @ 0x009A9040.
// Reset 0x009A9C80 stores this callback; FableWin 0x02FD5CF0 supplies its type.
void FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void* data)
{
    delete static_cast<CThreadedFile*>(data);
}
