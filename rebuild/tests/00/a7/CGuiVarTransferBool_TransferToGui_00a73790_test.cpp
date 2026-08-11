#include <cstdio>

struct GuiTarget { int received; bool got; };

struct CGuiVarTransferBool {
    GuiTarget* target;
    int        edxArg;
    char       pad8[4];
    bool       value;
    bool       dirty;
    void TransferToGui();
};

static int g_edx = 0;
static bool g_val = false;
static GuiTarget* g_self = 0;

void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value) {
    g_self = self;
    g_edx = edx;
    g_val = value;
    self->received = edx;
    self->got = value;
}

void CGuiVarTransferBool::TransferToGui() {
    GuiTarget_Set(this->target, this->edxArg, this->value);
    this->dirty = false;
}

int main() {
    GuiTarget tgt; tgt.received = 0; tgt.got = false;
    CGuiVarTransferBool obj;
    obj.target = &tgt;
    obj.edxArg = 12345;
    obj.value = true;
    obj.dirty = true;

    obj.TransferToGui();

    bool ok = (g_self == &tgt) && (g_edx == 12345) && (g_val == true)
              && (tgt.received == 12345) && (tgt.got == true)
              && (obj.dirty == false);
    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("FAIL\n"); return 1;
}