// CDataOutputStream::WriteSLONG(long) @ 0x00993EB0 (FableWin
// ?WriteSLONG@CDataOutputStream@@QAEXJ@Z): the four bytes of the value through the virtual
// Write. Out of line for the def members' binary transfer 0x00431102 and GFSerialiseVectorBinaryOut 0x00433227.
#include "fable_data_output_stream.h"

void CDataOutputStream::WriteSLONG(long value)
{
    Write(&value, 4);
}
