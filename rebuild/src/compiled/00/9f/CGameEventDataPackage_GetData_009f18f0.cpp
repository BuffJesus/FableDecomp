// CGameEventDataPackage::GetData @ 009F18F0 (FableWin member signature).
// FableWin locals: read_pos (unsigned long), byte_pointer (unsigned char*),
// bytes_to_read (long), overflow_check (__int64). The unsigned sum is
// evaluated before widening; keep its checked signed-long range and return.
// VC7.1 reproduces the complete 166-byte retail body.

#include "fable_game_event.h"
#include <string.h>
void CGameEventDataPackage::GetData(long size, void* data)
{
    unsigned long read_pos = StartPos;
    unsigned char* byte_pointer = static_cast<unsigned char*>(data);
    long bytes_to_read;
    if (read_pos + size > sizeof(Data))
    {
        bytes_to_read = sizeof(Data) - read_pos;
        __int64 overflow_check = read_pos + bytes_to_read;
        if (overflow_check < 0 || overflow_check > 0x7fffffff)
            return;
        memcpy(byte_pointer, Data + read_pos, bytes_to_read);
        byte_pointer += bytes_to_read;
        read_pos = 0;
        bytes_to_read = size - bytes_to_read;
        memcpy(byte_pointer, Data + read_pos, bytes_to_read);
        read_pos += bytes_to_read;
    }
    else
    {
        bytes_to_read = size;
        memcpy(byte_pointer, Data + read_pos, bytes_to_read);
        read_pos = (read_pos + bytes_to_read) % sizeof(Data);
    }
    StartPos = read_pos;
    Length -= size;
}
