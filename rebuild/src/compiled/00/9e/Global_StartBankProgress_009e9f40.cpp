#include "fable_ui_bank_factory.h"
void __fastcall FableUiStartBankProgress(const FableUiStringValue* text,bool flag,float progress,bool lastFlag)
{
    if(FableUiProgressDisplay)
        FableUiProgressDisplay->Vtable->Start(FableUiProgressDisplay,0,text,progress,flag,lastFlag);
}
