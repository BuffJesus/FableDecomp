#include "fable_ui_bank_ownership.h"

void __fastcall FableUiSetGraphicsBank(FableUiManagerView* manager,void*,FableUiBankReference bank)
{
    FableUiBankReference* owned=reinterpret_cast<FableUiBankReference*>(&manager->Configuration.GraphicsBank);
    FableUiShareBankReference(owned,0,bank.Data,bank.Info);
    // The incoming value already owns a reference; this is its native epilogue.
    FableUiReleaseBankReference(&bank,0);
}
