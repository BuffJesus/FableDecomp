#include <stdio.h>

struct CGuiVarTransferBool
{
    void* m_target;
    int   m_field;
    int   m_pad8;
    bool m_value;
    bool m_dirty;

    void TransferToGui();
};

// captured call arguments
static void* g_self;
static int   g_arg2;
static int   g_value;
static int   g_calls;

void __fastcall GuiSetBoolTarget(void* self, int arg2, bool value)
{
    g_self  = self;
    g_arg2  = arg2;
    g_value = value;
    ++g_calls;
}

void CGuiVarTransferBool::TransferToGui()
{
    GuiSetBoolTarget(this->m_target, this->m_field, this->m_value);
    this->m_dirty = 0;
}

int main()
{
    int dummyTarget = 0;
    CGuiVarTransferBool v;
    v.m_target = &dummyTarget;
    v.m_field  = 0x12345678;
    v.m_pad8   = 0;
    v.m_value  = 1;
    v.m_dirty  = 0xAB;

    g_calls = 0;
    v.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_self == &dummyTarget)
           && (g_arg2 == 0x12345678)
           && (g_value == 1)
           && (v.m_dirty == 0);

    // second run with value 0 to check zero-extension path
    v.m_value = 0;
    v.m_dirty = 0x77;
    v.TransferToGui();
    ok = ok && (g_value == 0) && (v.m_dirty == 0);

    printf(ok ? "TRANSFERTOGUI_OK\n" : "TRANSFERTOGUI_FAIL\n");
    return ok ? 0 : 1;
}