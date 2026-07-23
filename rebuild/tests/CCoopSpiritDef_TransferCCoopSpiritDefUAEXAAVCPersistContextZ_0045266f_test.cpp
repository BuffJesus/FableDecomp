#include <cstdio>

static int g_count = 0;
static void* g_fields[8];
static int g_overload[8];

struct CPersistContext {
    void TransferA(void* field) { g_overload[g_count] = 0; g_fields[g_count++] = field; }
    void TransferB(void* field) { g_overload[g_count] = 1; g_fields[g_count++] = field; }
};
struct CCoopSpiritDef {
    unsigned char _tail[0x3c];
    void CCoopSpiritDef_Transfer(CPersistContext& ctx);
};

void CCoopSpiritDef::CCoopSpiritDef_Transfer(CPersistContext& ctx)
{
    ctx.TransferA(this->_tail + 0x28);
    ctx.TransferA(this->_tail + 0x2c);
    ctx.TransferA(this->_tail + 0x30);
    ctx.TransferB(this->_tail + 0x38);
    ctx.TransferA(this->_tail + 0x34);
}

int main()
{
    CCoopSpiritDef def;
    CPersistContext ctx;
    def.CCoopSpiritDef_Transfer(ctx);
    unsigned char* base = def._tail;
    int off[5] = { 0x28, 0x2c, 0x30, 0x38, 0x34 };
    int ov[5]  = { 0, 0, 0, 1, 0 };
    if (g_count != 5) { std::printf("FAIL count=%d\n", g_count); return 1; }
    for (int i = 0; i < 5; i++) {
        if (g_fields[i] != base + off[i]) { std::printf("FAIL field %d\n", i); return 1; }
        if (g_overload[i] != ov[i]) { std::printf("FAIL overload %d\n", i); return 1; }
    }
    std::printf("CCoopSpiritDef_0045266f_TEST PASS\n");
    return 0;
}