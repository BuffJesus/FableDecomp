#include "fable_ui_bank_registry.h"
const char* __fastcall FableUiGetStringText(const FableUiStringValue* value,void*)
{ return value->Storage ? value->Storage->text : FableUiEmptyString; }
