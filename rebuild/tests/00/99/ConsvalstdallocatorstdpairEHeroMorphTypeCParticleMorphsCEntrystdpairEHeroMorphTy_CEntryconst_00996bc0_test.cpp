// Behaviour fixture for CMemoryDataOutputStream::ReserveSize (0x00996BC0): the
// data vector's reserve gets the size.
#include <stdio.h>
#include <string.h>

#include "fable_data_output_stream.h"

namespace
{
    int g_reserves;
    const void* g_reserveThis;
    size_t g_reserveSize;
}

namespace _STL
{
    template <>
    void vector<char>::reserve(size_t n)
    {
        ++g_reserves;
        g_reserveThis = this;
        g_reserveSize = n;
    }
}

int main()
{
    fable_u8 raw[sizeof(CMemoryDataOutputStream)];
    memset(raw, 0xA5, sizeof(raw));
    CMemoryDataOutputStream& stream = *reinterpret_cast<CMemoryDataOutputStream*>(raw);
    stream.ReserveSize(0x1234);
    stream.ReserveSize(0);
    const bool passed = g_reserves == 2 && g_reserveThis == &stream.Data && g_reserveSize == 0 &&
                        raw[0x14] == 0xA5;
    if (!passed)
    {
        printf("FABLETLC_CMEMORYDATAOUTPUTSTREAM_RESERVESIZE_00996BC0 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CMEMORYDATAOUTPUTSTREAM_RESERVESIZE_00996BC0_PASS\n");
    return 0;
}
