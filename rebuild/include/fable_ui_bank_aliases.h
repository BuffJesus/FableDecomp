#pragma once
#include "fable_ui_bank_decode.h"
// Retail CSmallVector<CCharString,8>: allocation stores its full count at Data[-1].
// The public count is a signed byte, distinct from that allocation cookie.
struct FableUiBankAliases { FableUiStringValue* Data; signed char Count; unsigned char Padding[3]; };
FABLE_STATIC_ASSERT(sizeof(FableUiBankAliases)==8);
void __fastcall FableUiClearBankAliases(FableUiBankAliases*,void*); // 009D2110
void __fastcall FableUiResizeBankAliases(FableUiBankAliases*,void*,unsigned); // 009D2160
void __fastcall FableUiCopyBankAliases(FableUiBankAliases*,void*,const FableUiBankAliases*); // 009D21D0
