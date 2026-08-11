#include <stdio.h>

struct GuiTarget;

void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value);

struct CGuiVarTransferBool
{
    GuiTarget* m_pTarget;
    int        m_field;
    int        m_pad8;
    bool       m_value;
    unsigned char m_done;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    GuiTarget_Set(m_pTarget, m_field, m_value);
    m_done = 0;
}

// capture args of the callee
static GuiTarget* g_self;
static int g_edx;
static bool g_value;

void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value)
{
    g_self = self;
    g_edx = edx;
    g_value = value;
}

int main()
{
    CGuiVarTransferBool o;
    o.m_pTarget = (GuiTarget*)0x1234;
    o.m_field = 99;
    o.m_pad8 = 0;
    o.m_value = 1;
    o.m_done = 0xAB;

    o.TransferToGui();

    if (g_self == (GuiTarget*)0x1234 && g_edx == 99 && g_value == true && o.m_done == 0)
        printf("XFERBOOL_OK\n");
    else
        printf("XFERBOOL_FAIL self=%p edx=%d val=%d done=%d\n", (void*)g_self, g_edx, g_value, o.m_done);
    return 0;
}