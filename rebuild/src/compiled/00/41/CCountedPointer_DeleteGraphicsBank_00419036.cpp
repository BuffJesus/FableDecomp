#include "fable_ui_bank_factory.h"
void __fastcall FableUiDestroyGraphicsBank(void* object)
{
    FableUiGraphicsBank* bank=static_cast<FableUiGraphicsBank*>(object);
    if(bank) bank->Vtable->Delete(bank,0,1);
}
