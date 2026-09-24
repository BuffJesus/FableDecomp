#pragma once
#include "fable_ui_strings.h"
struct FableUiWideStringValue { CWideStringData* Storage; };
// Wide storage is three pointers (begin, end, allocation end), then owners.
// Keep the shared legacy layout; unknown04/08 hold pointer values, not lengths.
void* __cdecl FableUiAllocateWideBuffer(unsigned); // 00BFEA0E
void __cdecl FableUiFreeWideBuffer(void*); // 00BFEA14
__declspec(noreturn) void __fastcall FableUiWideLengthError(CWideStringData*,void*); // 0099C550
void __fastcall FableUiConstructWideRange(CWideStringData*,void*,const wchar_t*,const wchar_t*,void*); // 0099C670
CWideStringData* __fastcall FableUiAppendWideRange(CWideStringData*,void*,const wchar_t*,const wchar_t*,void*); // 0099C7E0
CWideStringData* __fastcall FableUiAllocateWideData(FableUiWideStringValue*,void*,const wchar_t*,long); // 0099B3C0
void __fastcall FableUiUnassignWideString(FableUiWideStringValue*,void*); // 0099B4D0
void __fastcall FableUiDestroyWideString(FableUiWideStringValue*,void*); // 0099B510
void __fastcall FableUiMakeWideStringUnique(FableUiWideStringValue*,void*); // 0099B560
FableUiWideStringValue* __fastcall FableUiConstructWideText(FableUiWideStringValue*,void*,const wchar_t*); // 0099B6B0
FableUiWideStringValue* __fastcall FableUiCopyWideString(FableUiWideStringValue*,void*,const FableUiWideStringValue*); // 0099B720
FableUiWideStringValue* __fastcall FableUiAssignWideString(FableUiWideStringValue*,void*,const FableUiWideStringValue*); // 0099B7D0
FableUiWideStringValue* __fastcall FableUiAppendWideString(FableUiWideStringValue*,void*,const FableUiWideStringValue*); // 0099B8D0
FableUiWideStringValue* __fastcall FableUiConcatWideStrings(FableUiWideStringValue*,const FableUiWideStringValue*,const FableUiWideStringValue*); // 0099BE70
inline unsigned FableUiWideLength(const wchar_t* text)
{ unsigned length=0; while(text[length]) ++length; return length; }
