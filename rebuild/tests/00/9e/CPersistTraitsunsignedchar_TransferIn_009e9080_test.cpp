#include <cstdio>

struct CPersistContext;
struct CStringParser;

static unsigned char g_next;

unsigned char __fastcall PersistByteHelper(CPersistContext* context,
                                           CStringParser* parser,
                                           unsigned char* out)
{
    *out = g_next;
    return g_next;
}

struct CPersistTraits_uchar
{
    void TransferIn(CPersistContext* context, CStringParser* parser, unsigned char* out);
};

void CPersistTraits_uchar::TransferIn(CPersistContext* context,
                                      CStringParser* parser,
                                      unsigned char* out)
{
    PersistByteHelper(context, parser, (unsigned char*)&parser);
    *out = *(unsigned char*)&parser;
}

int main()
{
    CPersistTraits_uchar self;
    unsigned char dst = 0;
    g_next = 0xAB;
    self.TransferIn((CPersistContext*)0, (CStringParser*)0x1234, &dst);
    if (dst == 0xAB) {
        g_next = 0x07;
        self.TransferIn((CPersistContext*)0, (CStringParser*)0x1234, &dst);
        if (dst == 0x07) { printf("PARITY_OK\n"); return 0; }
    }
    printf("FAIL %d\n", dst);
    return 1;
}