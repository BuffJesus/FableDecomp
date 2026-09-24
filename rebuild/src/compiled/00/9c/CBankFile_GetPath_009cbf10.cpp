#include "fable_ui_bank_open.h"
FableUiWideStringValue* __fastcall FableUiGetBankPath(FableUiBankFileView* bank,void*,FableUiWideStringValue* out)
{
    void* file=bank->BankFile.Data;
    FableUiFilePathVtable* table=*static_cast<FableUiFilePathVtable**>(file);
    table->GetPath(file,0,out);
    return out;
}
