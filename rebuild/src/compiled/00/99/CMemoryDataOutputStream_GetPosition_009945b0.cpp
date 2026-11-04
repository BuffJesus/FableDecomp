// CMemoryDataOutputStream::GetPosition() const @ 0x009945B0 (FableWin
// ?GetPosition@CMemoryDataOutputStream@@UBEKXZ; vtable 0x0129A788 slot 2).
#include "fable_data_output_stream.h"

unsigned long CMemoryDataOutputStream::GetPosition() const
{
    return CurrentPosition;
}
