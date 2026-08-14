#include <stdio.h>
typedef int EInputKey;

struct CPersistContext;
struct CStringParser;
struct CPersistTraits;

// Model: the read routine writes a parsed key into *slot.
static int g_readCount = 0;
void __fastcall Persist_ReadValue(CPersistContext* ecx, CStringParser* edx, void* slot)
{
    (void)ecx; (void)edx;
    // Simulate: parse produced key value 0x2A into the slot.
    *(long*)slot = 0x2A;
    g_readCount++;
}

void __fastcall TransferIn(CPersistTraits* self, int edx_dummy,
                           CPersistContext* ctx, CStringParser* parser, EInputKey* out)
{
    (void)self; (void)edx_dummy;
    Persist_ReadValue(ctx, parser, &parser);
    *out = (EInputKey)(long)parser;
}

int main()
{
    EInputKey out = -1;
    CPersistContext* ctx = (CPersistContext*)0x1000;
    CStringParser* parser = (CStringParser*)0x2000;
    TransferIn((CPersistTraits*)0x3000, 0, ctx, parser, &out);
    if (g_readCount == 1 && out == 0x2A) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL count=%d out=%d\n", g_readCount, out);
    }
    return 0;
}