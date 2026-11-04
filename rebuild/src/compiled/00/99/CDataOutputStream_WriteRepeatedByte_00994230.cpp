// CDataOutputStream::WriteRepeatedByte(unsigned char, long) @ 0x00994230
// (FableWin ?WriteRepeatedByte@CDataOutputStream@@QAEXEJ@Z; CDefStringTable::AddString
// 0x009D4F90 writes the string stream's 4-byte zero header with it).
#include "fable_data_output_stream.h"
#include <string.h>

// Write `count` copies of the byte, a buffer of up to 256 at a time.
void CDataOutputStream::WriteRepeatedByte(unsigned char value, long count)
{
    unsigned char buffer[256];
    memset(buffer, value, count < 256 ? count : 256);
    while (count > 0)
    {
        long chunk = count < 256 ? count : 256;
        Write(buffer, chunk);
        count -= chunk;
    }
}