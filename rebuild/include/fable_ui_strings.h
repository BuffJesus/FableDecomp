#pragma once
#include "fable_string.h"

// Non-owning ABI view of CCharString. Explicit operations below preserve native
// call/temporary lifetime boundaries when integrating recovered callers.
struct FableUiStringValue { CCharStringData* Storage; };
FABLE_STATIC_ASSERT(sizeof(FableUiStringValue)==4);
void* __cdecl FableUiAllocateStringRecord(unsigned);
void __cdecl FableUiFreeStringRecord(void*);
void* __cdecl FableUiAllocateStringBuffer(unsigned);
void __cdecl FableUiFreeStringBuffer(void*);
void __fastcall FableUiAssignStringBytes(CCharStringData*,void*,const char*,unsigned);
CCharStringData* __fastcall FableUiConstructStringData(CCharStringData*,void*,const char*);
CCharStringData* __fastcall FableUiAllocateStringData(FableUiStringValue*,void*,const char*,long);
void __fastcall FableUiUnassignString(FableUiStringValue*,void*);
FableUiStringValue* __fastcall FableUiConstructBankName(FableUiStringValue*,void*,const char*,long);
void __fastcall FableUiDestroyBankName(FableUiStringValue*,void*);
FableUiStringValue* __fastcall FableUiConstructEmptyString(FableUiStringValue*,void*);
FableUiStringValue* __fastcall FableUiCopyString(FableUiStringValue*,void*,const FableUiStringValue*);
FableUiStringValue* __fastcall FableUiAssignString(FableUiStringValue*,void*,const FableUiStringValue*);
void __fastcall FableUiMakeStringUnique(FableUiStringValue*,void*); // 0099EAF0
void __fastcall FableUiReserveStringBytes(CCharStringData*,void*,unsigned); // 009A01C0
void __fastcall FableUiAppendStringBytes(CCharStringData*,void*,const char*,unsigned); // 009A04E0
FableUiStringValue* __fastcall FableUiAppendStringLiteral(FableUiStringValue*,void*,const char*); // 0099F100
FableUiStringValue* __fastcall FableUiConcatStringLiteral(FableUiStringValue*,const FableUiStringValue*,const char*); // 0099F600
inline const char* FableUiStringText(const FableUiStringValue* value)
{ return value->Storage ? value->Storage->text : ""; }
