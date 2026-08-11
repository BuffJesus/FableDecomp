#include <cstdio>

void __fastcall GuiVarBool_Apply(void* self, int arg2, bool value);

struct CGuiVarTransferBool
{
    void*         pTarget;
    int           arg2;
    int           pad8;
    bool          value;
    unsigned char dirty;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiVarBool_Apply(this->pTarget, this->arg2, this->value);
    this->dirty = 0;
}

static void* g_self;
static int   g_arg2;
static bool  g_value;
static int   g_calls;

void __fastcall GuiVarBool_Apply(void* self, int arg2, bool value)
{
    g_self = self;
    g_arg2 = arg2;
    g_value = value;
    ++g_calls;
}

int main()
{
    int sentinel = 0x1234;
    CGuiVarTransferBool o;
    o.pTarget = &sentinel;
    o.arg2 = 99;
    o.pad8 = 0;
    o.value = true;
    o.dirty = 1;

    o.TransferToGui();

    bool ok = (g_calls == 1) &&
              (g_self == &sentinel) &&
              (g_arg2 == 99) &&
              (g_value == true) &&
              (o.dirty == 0);

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("TRANSFERBOOL_FAIL\n");
    return 1;
}