#include <cstdio>

struct CXfer;

static int g_base_called = 0;
static int g_field_called = 0;
static void* g_field_obj = 0;
static void* g_saved_self = 0;
static void* g_saved_xfer = 0;

struct CBaseDef {
    void Transfer(CXfer* xfer);
};

struct CXfer {
    void Transfer(void* obj);
};

struct CThingSwitchDef : public CBaseDef {
    char pad[0x70];
    CXfer sub;
    void Transfer(CXfer* xfer);
};

void CBaseDef::Transfer(CXfer* xfer)
{
    g_base_called = 1;
    g_saved_self = this;
    g_saved_xfer = xfer;
}

void CXfer::Transfer(void* obj)
{
    g_field_called = 1;
    g_field_obj = obj;
}

void CThingSwitchDef::Transfer(CXfer* xfer)
{
    CBaseDef::Transfer(xfer);
    xfer->Transfer(&this->sub);
}

int main()
{
    CThingSwitchDef def;
    CXfer x;
    def.Transfer(&x);
    if (g_base_called && g_field_called &&
        g_saved_self == (void*)&def && g_saved_xfer == (void*)&x &&
        g_field_obj == (void*)((char*)&def + 0x70)) {
        std::printf("CThingSwitchDef_00454f15_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL base=%d field=%d\n", g_base_called, g_field_called);
    return 1;
}