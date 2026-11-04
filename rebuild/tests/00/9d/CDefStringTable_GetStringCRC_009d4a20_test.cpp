// Behaviour fixture for CDefStringTable::GetStringCRC (0x009D4A20).
#include <stdio.h>
#include <string.h>

#include "fable_def_string.h"

namespace
{
    unsigned long g_data[4] = { 0x11111111, 0xCAFEBABE, 0x22222222, 0x33333333 };
    const void* g_peekThis;
}

const void* CMemoryDataOutputStream::PeekData() const
{
    g_peekThis = this;
    return g_data;
}

int main()
{
    fable_u8 raw[sizeof(CDefStringTable)];
    memset(raw, 0, sizeof(raw));
    const CDefStringTable& table = *reinterpret_cast<const CDefStringTable*>(raw);
    const bool passed = table.GetStringCRC(-1) == 0 && g_peekThis == 0 &&
                        table.GetStringCRC(4) == 0xCAFEBABE && g_peekThis == &table.StringStream &&
                        table.GetStringCRC(12) == 0x33333333;
    if (!passed)
    {
        printf("FABLETLC_CDEFSTRINGTABLE_GETSTRINGCRC_009D4A20 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CDEFSTRINGTABLE_GETSTRINGCRC_009D4A20_PASS\n");
    return 0;
}
