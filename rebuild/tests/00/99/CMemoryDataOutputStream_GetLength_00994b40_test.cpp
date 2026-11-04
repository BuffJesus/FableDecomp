// Behaviour of CMemoryDataOutputStream::GetLength 0x00994B40: the size of Data
// (end - begin), independent of the current position.
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char store[sizeof(CMemoryDataOutputStream)];
    memset(store, 0, sizeof(store));
    CMemoryDataOutputStream* s = reinterpret_cast<CMemoryDataOutputStream*>(store);
    static char bytes[32];
    if (s->GetLength() != 0) { puts("empty"); return 1; }
    s->Data._M_start = bytes;
    s->Data._M_finish = bytes + 21;
    s->Data._M_end_of_storage._M_data = bytes + 32;
    s->CurrentPosition = 3;
    if (s->GetLength() != 21) { printf("got %lu\n", s->GetLength()); return 1; }
    puts("FABLETLC_CMEMORYDATAOUTPUTSTREAM_GETLENGTH_00994B40_PASS");
    return 0;
}