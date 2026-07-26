#include <cstdio>

static void* g_captured_this = 0;
static void* g_captured_arg = 0;

struct CTransferContext {
    int dummy;
    int Xfer(void* p);
};

struct CSpecialAbilitiesForcePushDataDef {
    unsigned char _tail[0x2c];
    void Transfer(CTransferContext* ctx);
};

int CTransferContext::Xfer(void* p)
{
    g_captured_this = this;
    g_captured_arg = p;
    return 0;
}

void CSpecialAbilitiesForcePushDataDef::Transfer(CTransferContext* ctx)
{
    ctx->Xfer(this->_tail + 0x28);
}

int main()
{
    CSpecialAbilitiesForcePushDataDef obj;
    CTransferContext ctx;
    obj.Transfer(&ctx);
    if (g_captured_this == (void*)&ctx &&
        g_captured_arg == (void*)((unsigned char*)&obj + 0x28)) {
        std::printf("CSpecialAbilitiesForcePushDataDef_004525a3_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL this=%p arg=%p\n", g_captured_this, g_captured_arg);
    return 1;
}