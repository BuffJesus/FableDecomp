#include <cstdio>
#include "rebuild_abi.h"

struct CGuiVarTransferBool {
    void*        field_0;
    int          field_4;
    char         pad_8[4];
    unsigned char value;
    unsigned char flag;
};

extern void __fastcall CGuiVarTransferBool_Sink(void* ecx, int edx, unsigned char val);
void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* thisptr);

static void* g_ecx;
static int   g_edx;
static unsigned char g_val;
void __fastcall CGuiVarTransferBool_Sink(void* ecx, int edx, unsigned char val) {
    g_ecx = ecx; g_edx = edx; g_val = val;
}

int main() {
    CGuiVarTransferBool o;
    int sentinel = 0;
    o.field_0 = &sentinel;
    o.field_4 = 0x1234;
    o.value = 0xAB;
    o.flag = 1;
    CGuiVarTransferBool_TransferToGui(&o);
    if (g_ecx == &sentinel && g_edx == 0x1234 && g_val == 0xAB && o.flag == 0) {
        printf("OK_470cd8_TG\n");
    } else {
        printf("FAIL_470cd8\n");
    }
    return 0;
}