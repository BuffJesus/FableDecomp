#include <stdio.h>

struct CGuiVarTransferBool {
    void* pGui;
    void* pVar;
    int   pad8;
    unsigned char curValue;
    unsigned char dirty;
};

static void* g_gui;
static void* g_var;
static int   g_value = -1;

extern void __fastcall GuiVarBoolHelper(void* pGui, void* pVar, unsigned char value);
void __fastcall GuiVarBoolHelper(void* pGui, void* pVar, unsigned char value) {
    g_gui = pGui; g_var = pVar; g_value = value;
}

extern void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self);

int main() {
    CGuiVarTransferBool o;
    int guiObj = 11, varObj = 22;
    o.pGui = &guiObj;
    o.pVar = &varObj;
    o.pad8 = 0;
    o.curValue = 1;
    o.dirty = 0xFF;

    CGuiVarTransferBool_TransferToGui(&o);

    if (g_gui == &guiObj && g_var == &varObj && g_value == 1 && o.dirty == 0)
        printf("0077f4c0_TEST PASS\n");
    else
        printf("FAIL gui=%p var=%p val=%d dirty=%d\n", g_gui, g_var, g_value, (int)o.dirty);
    return 0;
}