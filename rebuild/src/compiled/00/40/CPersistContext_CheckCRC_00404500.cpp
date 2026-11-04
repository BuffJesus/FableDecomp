// CPersistContext::CheckCRC(const char*) @ 0x00404500 (FableWin
// ?CheckCRC@CPersistContext@@IAEXPBD@Z). Binary load skips the stored CRC of
// the member name (read and dropped: retail does not compare it); binary save
// writes the name's CRC. Other modes do nothing. The unit calls the
// out-of-line WriteULONG (0x00993EA0).
#include "fable_persist_context.h"
#include "fable_data_input_stream.h"
#include "fable_string.h"

void CPersistContext::CheckCRC(const char* name)
{
    if (Mode == MODE_BINARY_LOAD)
    {
        PLoadStream->ReadValue<unsigned long>();
    }
    else if (Mode == MODE_BINARY_SAVE)
    {
        PSaveStream->WriteULONG(CCharString(name).GetCRC());
    }
}