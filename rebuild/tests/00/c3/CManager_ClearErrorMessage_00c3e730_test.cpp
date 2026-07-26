#include <cstdio>

static int g_close = 0;
static int g_rel = 0;
static int g_arg = -1;

struct IErr {
    virtual void Release(int b) = 0;
    virtual void Slot1() = 0;
    virtual void Close() = 0;
};
struct Impl : IErr {
    virtual void Release(int b) { g_rel++; g_arg = b; }
    virtual void Slot1() {}
    virtual void Close() { g_close++; }
};
struct CManager { char pad[0x17c]; IErr* err; };

void __fastcall CManager_ClearErrorMessage(CManager* self, int edx_dummy, int arg)
{
    IErr* e = self->err;
    e->Close();
    IErr* e2 = self->err;
    if (e2)
        e2->Release(1);
    self->err = 0;
}

int main()
{
    Impl obj;
    CManager mgr;
    mgr.err = &obj;
    CManager_ClearErrorMessage(&mgr, 0, 0);
    if (g_close == 1 && g_rel == 1 && g_arg == 1 && mgr.err == 0) {
        std::printf("CManager_00c3e730_TEST_PASS\n");
        return 0;
    }
    std::printf("FAIL close=%d rel=%d arg=%d err=%p\n", g_close, g_rel, g_arg, (void*)mgr.err);
    return 1;
}