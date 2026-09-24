#pragma once
#include "fable_ui_bank_stream.h"
void* __cdecl FableUiAllocateArchiveArray(unsigned); // 00BFEA0E
void __cdecl FableUiFreeArchiveArray(void*); // 00BFEA14
FableUiGraphicArray* __fastcall FableUiConstructArchiveBytes(FableUiGraphicArray*,void*,unsigned); // 00411910
FableUiGraphicArray* __fastcall FableUiConstructArchivePairs(FableUiGraphicArray*,void*,unsigned); // 009D2AF0
FableUiStringValue* __fastcall FableUiReadArchiveString(CFileDataInputStream*,void*,FableUiStringValue*); // 00996390
