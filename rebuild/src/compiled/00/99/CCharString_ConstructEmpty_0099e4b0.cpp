#include "fable_ui_strings.h"
FableUiStringValue* __fastcall FableUiConstructEmptyString(FableUiStringValue* value,void*)
{ value->Storage=0; ++g_CCharStringInstanceCount_013BD800; return value; }
