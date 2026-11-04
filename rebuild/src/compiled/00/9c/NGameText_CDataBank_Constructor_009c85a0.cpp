#include "fable_game_text_bank.h"

// NGameText::CDataBank::CDataBank() @ 0x009C85A0.
NGameText::CDataBank::CDataBank()
    : Seed(0)
{
    ResourceList.SetMaximumMemory(0x10000);
    ResourceList.UnloadDelay = 0;
}
