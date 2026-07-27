#include "fable_boot.h"

#include <stdio.h>

struct CDataBank
{
    int identity;
};

FableProgressTextBankSlot*
    g_FableProgressTextBankSlot_013B86A0 = 0;
FableGameTextBankSlot*
    g_FableGameTextBankSlot_013B871C = 0;

int main()
{
    unsigned char displayStorage[sizeof(CProgressDisplay)] = {0};
    CProgressDisplay* display =
        reinterpret_cast<CProgressDisplay*>(displayStorage);

    CDataBank progressBank = {1};
    CDataBank gameBank = {2};
    FableProgressTextBankSlot progressSlot = {};
    FableGameTextBankSlot gameSlot = {};

    if (display->GetPTextBank() != 0)
        return 1;

    g_FableProgressTextBankSlot_013B86A0 = &progressSlot;
    g_FableGameTextBankSlot_013B871C = &gameSlot;
    if (display->GetPTextBank() != 0)
        return 2;

    gameSlot.textBank60 = &gameBank;
    if (display->GetPTextBank() != &gameBank)
        return 3;

    progressSlot.textBank14 = &progressBank;
    if (display->GetPTextBank() != &progressBank)
        return 4;

    g_FableProgressTextBankSlot_013B86A0 = 0;
    if (display->GetPTextBank() != &gameBank)
        return 5;

    printf("FABLETLC_PROGRESS_TEXT_BANK_BEHAVIOR PASS\n");
    return 0;
}