// Behaviour of CPersistContext::Init(CDataOutputStream&, bool) 0x009BAA50:
// binary-save mode into the stream, no load stream, no definition manager,
// CRC check off; the flag is not stored and nothing else is written.
#include "fable_persist_context.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char store[sizeof(CPersistContext)];
    static unsigned char streamStore[64];
    CPersistContext& c = *reinterpret_cast<CPersistContext*>(store);
    for (int pass = 0; pass < 2; ++pass)
    {
        memset(store, pass ? 0xFF : 0x11, sizeof(store));
        unsigned char before[sizeof(CPersistContext)];
        memcpy(before, store, sizeof(store));
        c.Init(*reinterpret_cast<CDataOutputStream*>(streamStore), pass == 1);
        if (!(c.Mode == CPersistContext::MODE_BINARY_SAVE && c.PSaveStream == (void*)streamStore && c.PLoadStream == 0 && c.PDefinitionManager == 0 && !c.UseCRCCheck)) { printf("pass %d: fields\n", pass); return 1; }
        // Everything else untouched.
        for (unsigned i = 0; i < sizeof(store); ++i)
        {
            bool written = (i < 4) || i == 0x15 || (i >= 0x18 && i < 0x1C) || (i >= 0x24 && i < 0x2C);
            if (!written && store[i] != before[i]) { printf("pass %d: byte %u\n", pass, i); return 1; }
        }
    }
    puts("FABLETLC_CPERSISTCONTEXT_INIT_SAVE_009BAA50_PASS");
    return 0;
}