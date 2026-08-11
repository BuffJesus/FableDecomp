#include <stdio.h>

struct GuiTarget;

void __fastcall GuiBoolStore(GuiTarget* self, int id, bool value);

struct CGuiVarTransferBool
{
    GuiTarget*    m_target;
    int           m_id;
    int           m_pad8;
    bool          m_value;
    bool          m_dirty;

    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiBoolStore(this->m_target, this->m_id, this->m_value);
    this->m_dirty = false;
}

// Local stand-in for callee: records args.
static void* g_seenTarget;
static int   g_seenId;
static bool  g_seenValue;
static int   g_calls;

void __fastcall GuiBoolStore(GuiTarget* self, int id, bool value)
{
    g_seenTarget = (void*)self;
    g_seenId     = id;
    g_seenValue  = value;
    g_calls++;
}

int main()
{
    CGuiVarTransferBool v;
    GuiTarget* fake = (GuiTarget*)0x12345678;
    v.m_target = fake;
    v.m_id     = 77;
    v.m_pad8   = 0;
    v.m_value  = true;
    v.m_dirty  = true;

    v.TransferToGui();

    bool ok = true;
    if (g_seenTarget != (void*)fake) ok = false;
    if (g_seenId != 77) ok = false;
    if (g_seenValue != true) ok = false;
    if (v.m_dirty != false) ok = false;

    // second case: value false
    v.m_value = false;
    v.m_dirty = true;
    v.TransferToGui();
    if (g_seenValue != false) ok = false;
    if (v.m_dirty != false) ok = false;
    if (g_calls != 2) ok = false;

    printf(ok ? "TRANSFERBOOL_OK\n" : "TRANSFERBOOL_FAIL\n");
    return ok ? 0 : 1;
}