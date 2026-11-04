// CDataOutputStream::WriteULONG(unsigned long) @ 0x00993EA0 (FableWin
// ?WriteULONG@CDataOutputStream@@QAEXK@Z): the four bytes of the value through the virtual
// Write. Out of line for CPersistContext::CheckCRC 0x00404500.
#include "fable_data_output_stream.h"

void CDataOutputStream::WriteULONG(unsigned long value)
{
    Write(&value, 4);
}
