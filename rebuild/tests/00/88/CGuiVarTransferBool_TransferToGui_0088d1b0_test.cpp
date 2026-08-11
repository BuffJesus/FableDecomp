#include <cstdio>

extern "C" void __fastcall CGuiVarTransferBool_Setter(void* pThis, void* edxVal, unsigned char value);

struct CGuiVarTransferBool
{
    void*         field_0;
    void*         field_4;
    int           field_8;
    unsigned char field_C;
    unsigned char field_D;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    CGuiVarTransferBool_Setter(this->field_0, this->field_4, this->field_C);
    this->field_D = 0;
}

// Local stub of the callee for the standalone behaviour test.
static void*        g_recvThis = 0;
static void*        g_recvEdx  = 0;
static int          g_recvVal  = -999;

extern "C" void __fastcall CGuiVarTransferBool_Setter(void* pThis, void* edxVal, unsigned char value)
{
    g_recvThis = pThis;
    g_recvEdx  = edxVal;
    g_recvVal  = (int)value;
}

int main()
{
    int dummyA = 0, dummyB = 0;
    CGuiVarTransferBool o;
    o.field_0 = &dummyA;
    o.field_4 = &dummyB;
    o.field_8 = 0x1234;
    o.field_C = 1;
    o.field_D = 0x55;

    o.TransferToGui();

    bool ok = true;
    if (g_recvThis != &dummyA) ok = false;
    if (g_recvEdx  != &dummyB) ok = false;
    if (g_recvVal  != 1)       ok = false;
    if (o.field_D  != 0)       ok = false;

    // second case: field_C = 0
    o.field_C = 0;
    o.field_D = 0x77;
    o.TransferToGui();
    if (g_recvVal != 0) ok = false;
    if (o.field_D != 0) ok = false;

    if (ok) { printf("TRANSFERBOOL_OK\n"); return 0; }
    printf("TRANSFERBOOL_FAIL this=%p edx=%p val=%d d=%d\n",
           g_recvThis, g_recvEdx, g_recvVal, (int)o.field_D);
    return 1;
}