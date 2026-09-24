#include "fable_ui_bank_runtime.h"
void __fastcall FableUiDeleteTextureManager(void* object)
{
    FableUiGraphicsBank* view=static_cast<FableUiGraphicsBank*>(object);
    if(view) view->Vtable->Delete(view,0,1);
}
