#pragma once
#include "fable_ui_transform.h"
struct FableUiSystemManagerView
{
    unsigned char Unrecovered00[0x60];
    void* Display;
    void* Unrecovered64;
    void* Unrecovered68;
    void* GraphicsBankManager;
};
struct FableUiDisplayExtent { int Width,Height; };
FABLE_STATIC_ASSERT(offsetof(FableUiSystemManagerView,Display)==0x60);
FABLE_STATIC_ASSERT(offsetof(FableUiSystemManagerView,GraphicsBankManager)==0x6C);
// Platform/engine services 009A4EC0 and 009BEDC0.
FableUiSystemManagerView* __cdecl FableUiGetSystemManager();
void __fastcall FableUiQueryDisplayExtent(void*,void*,FableUiDisplayExtent*);
void __fastcall FableUiSetRelativeCoordinates(bool);
