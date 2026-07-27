#include "fable_boot.h"

CDataBank* CProgressDisplay::GetPTextBank() const
{
    CDataBank* bank;

    if (g_FableProgressTextBankSlot_013B86A0 != 0)
    {
        bank =
            g_FableProgressTextBankSlot_013B86A0->textBank14;
        if (bank != 0)
            return bank;
    }

    if (g_FableGameTextBankSlot_013B871C != 0)
    {
        bank = g_FableGameTextBankSlot_013B871C->textBank60;
        if (bank != 0)
            return bank;
    }

    return 0;
}