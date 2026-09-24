#include "fable_ui_wide_strings.h"
void __fastcall FableUiDestroyWideString(FableUiWideStringValue* value,void*)
{ FableUiUnassignWideString(value,0); --g_CWideStringInstanceCount_013BCA20; }
