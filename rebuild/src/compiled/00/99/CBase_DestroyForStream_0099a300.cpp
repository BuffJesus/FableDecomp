#include "fable_ui_bank_stream.h"
void __fastcall FableUiDestroyStreamBase(void* base,void*)
{ *static_cast<void**>(base)=FableUiDestroyedBaseVtable; }
