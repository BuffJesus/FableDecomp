#pragma once
#include "fable_ui_manager_construction.h"
#include "fable_reference_count.h"

FABLE_STATIC_ASSERT(sizeof(FableUiBankReference)==8);
FABLE_STATIC_ASSERT(sizeof(FableReferenceCount)==12);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView,GraphicsBank)==0x10);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView,GraphicsBankReference)==0x14);

// Shared retail operator-delete boundary (00BFE9BC), also used by child ownership.
void __cdecl FableUiDeleteReference(FableReferenceCount*);
// 00419108 clears the reference after releasing it; 00419134 compares Info,
// not Data. The latter takes two raw words and does not consume an argument owner.
void __fastcall FableUiReleaseBankReference(FableUiBankReference*,void*);
void __fastcall FableUiShareBankReference(FableUiBankReference*,void*,void*,void*);
