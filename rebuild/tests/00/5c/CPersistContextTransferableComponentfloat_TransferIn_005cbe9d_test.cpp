#include <stdio.h>

class CPersistContext;
class CStringParser;

struct TransferHelper {
    unsigned int m_dword;
    void Apply(CPersistContext* ctx, CStringParser* parser, unsigned int value);
};

struct CPersistContext_TransferableComponent_float {
    void* m_vtbl;
    unsigned int m_value;
    void TransferIn(CPersistContext* ctx, CStringParser* parser);
};

// Capture what TransferIn forwards through the helper.
static unsigned int g_seen_value = 0;
static void*        g_seen_ctx   = 0;
static void*        g_seen_parser= 0;
static int          g_seen_init  = -1;

void TransferHelper::Apply(CPersistContext* ctx, CStringParser* parser, unsigned int value)
{
    g_seen_init   = (int)m_dword;   // must be 0 (zero-inited temp)
    g_seen_ctx    = (void*)ctx;
    g_seen_parser = (void*)parser;
    g_seen_value  = value;
}

void CPersistContext_TransferableComponent_float::TransferIn(CPersistContext* ctx, CStringParser* parser)
{
    TransferHelper tmp;
    tmp.m_dword = 0;
    tmp.Apply(ctx, parser, this->m_value);
}

int main()
{
    CPersistContext_TransferableComponent_float obj;
    obj.m_vtbl  = (void*)0xDEADBEEF;
    obj.m_value = 0x40490FDB; // bits of 3.14159f

    CPersistContext* ctx    = (CPersistContext*)0x11112222;
    CStringParser*   parser = (CStringParser*)0x33334444;

    obj.TransferIn(ctx, parser);

    int ok = (g_seen_init == 0)
          && (g_seen_ctx    == (void*)0x11112222)
          && (g_seen_parser == (void*)0x33334444)
          && (g_seen_value  == 0x40490FDB);

    if (ok) { printf("TRANSFERIN_OK\n"); return 0; }
    printf("TRANSFERIN_FAIL init=%d val=%08X\n", g_seen_init, g_seen_value);
    return 1;
}