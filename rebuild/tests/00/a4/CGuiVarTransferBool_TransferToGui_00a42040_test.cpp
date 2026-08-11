#include <cstdio>

struct CGuiVarTransferBool
{
    void* target;
    int   handle;
    int   pad8;
    bool  value;
    bool  changed;
    void TransferToGui();
};

// Capture what the fastcall helper received.
static void*        g_obj;
static int          g_handle;
static int          g_value;
static int          g_calls;

void __fastcall GuiVarSetBool(void* obj, int handle, bool value)
{
    g_obj = obj;
    g_handle = handle;
    g_value = value ? 1 : 0;
    ++g_calls;
}

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarSetBool(this->target, this->handle, this->value);
    this->changed = false;
}

int main()
{
    int dummyObj = 0;
    CGuiVarTransferBool v;
    v.target = &dummyObj;
    v.handle = 0x1234;
    v.pad8 = 0;
    v.value = true;
    v.changed = true;

    v.TransferToGui();

    bool ok = (g_calls == 1)
           && (g_obj == &dummyObj)
           && (g_handle == 0x1234)
           && (g_value == 1)
           && (v.changed == false);

    // second case: false transfers as 0, changed cleared
    v.value = false;
    v.changed = true;
    v.TransferToGui();
    ok = ok && (g_value == 0) && (v.changed == false) && (g_calls == 2);

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("FAIL calls=%d handle=%d value=%d changed=%d\n", g_calls, g_handle, g_value, (int)v.changed);
    return 1;
}