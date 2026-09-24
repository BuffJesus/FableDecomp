#include "fable_ui_bank_runtime.h"
void* __fastcall FableUiConstructBase(void* object,void*)
{ *static_cast<void**>(object)=FableUiBaseVtable; return object; }
