#pragma once
#include "fable_ui_bank_storage.h"
#include "fable_ui_bank_aliases.h"
struct FableUiBankUpdateEntry
{
    unsigned Metadata,Length;
    void* Bytes;
    FableUiBankAliases Aliases;
    bool Unrecovered14,Unrecovered15;
    unsigned char Padding[2];
};
FABLE_STATIC_ASSERT(sizeof(FableUiBankUpdateEntry)==24);
// Remaining index/finalization services are explicit integration boundaries.
void __fastcall FableUiRegisterBankEntry(FableUiBankFileView*,void*,unsigned,const FableUiStringValue*,const FableUiStringValue*); // 009CE050
void __fastcall FableUiSortBankChecksums(void*,void*,unsigned char); // 009B85A0: ECX begin, EDX end
void __fastcall FableUiCompactBankChecksums(FableUiGraphicArray*,void*); // 009B7B10
void __fastcall FableUiPackBankRuntime(FableUiBankFileView*,void*); // 009CD740
