#include "fable_ui_bank_storage.h"
void __fastcall FableUiDestroyStringRange(FableUiStringValue* first,FableUiStringValue* last)
{ for(;first!=last;++first) FableUiDestroyBankName(first,0); }
