#include <cstdio>

struct GuiTarget { int tag; };

static GuiTarget* g_seenSelf = 0;
static int g_seenEdx = -1;
static int g_seenBool = -1;

struct CGuiVarTransferBool {
    GuiTarget* target;
    int        field4;
    char       pad8[4];
    bool       value;
    char       flag;
};

void __fastcall Helper_5fa3dd(GuiTarget* self, int edxArg, bool boolArg) {
    g_seenSelf = self;
    g_seenEdx  = edxArg;
    g_seenBool = boolArg ? 1 : 0;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self);

int main() {
    GuiTarget t; t.tag = 77;
    CGuiVarTransferBool o;
    o.target = &t;
    o.field4 = 0x1234;
    o.value  = true;
    o.flag   = (char)0x55;

    TransferToGui(&o);

    if (g_seenSelf == &t && g_seenEdx == 0x1234 && g_seenBool == 1 && o.flag == 0)
        printf("005fa47d_TEST PASS\n");
    else
        printf("FAIL self=%p edx=%x b=%d flag=%d\n", (void*)g_seenSelf, g_seenEdx, g_seenBool, (int)o.flag);
    return 0;
}