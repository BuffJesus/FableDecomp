// CDefStringTable::GetStringCRC(long) const @ 0x009D4A20 (FableWin
// ?GetStringCRC@CDefStringTable@@QBEKJ@Z): each table entry starts with the
// CRC of its text; no entry (-1) has CRC 0.
#include "fable_def_string.h"

unsigned long CDefStringTable::GetStringCRC(long tablePos) const
{
    if (tablePos == -1)
        return 0;
    const fable_u8* entry = static_cast<const fable_u8*>(StringStream.PeekData());
    entry += tablePos;
    return *reinterpret_cast<const unsigned long*>(entry);
}
