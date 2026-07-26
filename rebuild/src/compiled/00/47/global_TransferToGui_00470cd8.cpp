#include "rebuild_abi.h"

struct CGuiVarTransferBool {
    void*        field_0;   // +0x00 -> ecx for callee
    int          field_4;   // +0x04 -> edx for callee
    char         pad_8[4];  // +0x08
    unsigned char value;    // +0x0c -> pushed byte arg
    unsigned char flag;     // +0x0d -> cleared to 0
};

// Callee: ecx = this->field_0, edx = this->field_4, one stack byte arg.
extern void __fastcall CGuiVarTransferBool_Sink(void* ecx, int edx, unsigned char val);

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* thisptr)
{
    CGuiVarTransferBool_Sink(thisptr->field_0, thisptr->field_4, (unsigned char)thisptr->value);
    thisptr->flag = 0;
}