// Behaviour fixture for CDataOutputStream::CopyFrom (0x00996970): forwards to
// the input stream's CopyTo with this stream and the size.
#include <stdio.h>
#include <string.h>

#include "fable_data_output_stream.h"
#include "fable_data_input_stream.h"

namespace
{
    int g_copies;
    const void* g_from;
    const void* g_to;
    unsigned long g_size;
}

void CDataInputStream::CopyTo(CDataOutputStream& stream, unsigned long size)
{
    ++g_copies;
    g_from = this;
    g_to = &stream;
    g_size = size;
}

int main()
{
    fable_u8 in[sizeof(CDataInputStream)];
    fable_u8 out[sizeof(CMemoryDataOutputStream)];
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    CDataOutputStream& output = *reinterpret_cast<CDataOutputStream*>(out);
    output.CopyFrom(*reinterpret_cast<CDataInputStream*>(in), 77);
    const bool passed = g_copies == 1 && g_from == in && g_to == out && g_size == 77;
    if (!passed)
    {
        printf("FABLETLC_CDATAOUTPUTSTREAM_COPYFROM_00996970 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CDATAOUTPUTSTREAM_COPYFROM_00996970_PASS\n");
    return 0;
}