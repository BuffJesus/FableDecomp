#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"

extern "C" fable_u8 g_FableGameTextDataBankVTable_0129B4D4 = 0;
extern "C" fable_u8 g_FableGameTextResourceBankVTable_0129B528 = 0;

namespace
{
    enum Event
    {
        kBankFileConstruct,
        kResourceBankConstruct,
        kSetMaximumMemory
    };

    fable_u8* g_object;
    Event g_events[3];
    unsigned int g_eventCount;
    bool g_argumentsValid;
    bool g_initializationVisible;

    void Record(Event event)
    {
        if (g_eventCount < 3)
        {
            g_events[g_eventCount] = event;
        }
        ++g_eventCount;
    }

    fable_u32 ReadU32(unsigned int offset)
    {
        return *reinterpret_cast<fable_u32*>(g_object + offset);
    }
}

extern "C" void FABLE_FASTCALL
FableGameTextBankFileConstruct_009C85A0(
    void* bank,
    void*)
{
    Record(kBankFileConstruct);
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_object;
}

extern "C" void FABLE_FASTCALL
FableGameTextResourceBankConstruct_009C85A0(
    void* bank,
    void*)
{
    Record(kResourceBankConstruct);
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_object + 0x164;
    memset(bank, 0x5A, 0x7C);
}

extern "C" void FABLE_FASTCALL
FableGameTextSetMaximumMemory_009C85A0(
    void* resourceList,
    void*,
    fable_u32 maximumMemory)
{
    Record(kSetMaximumMemory);
    g_argumentsValid =
        g_argumentsValid &&
        resourceList == g_object + 0x168 &&
        maximumMemory == 0x10000;

    g_initializationVisible =
        ReadU32(0x000) ==
            reinterpret_cast<fable_u32>(
                &g_FableGameTextDataBankVTable_0129B4D4) &&
        ReadU32(0x164) ==
            reinterpret_cast<fable_u32>(
                &g_FableGameTextResourceBankVTable_0129B528) &&
        ReadU32(0x210) == 0;
    for (unsigned int offset = 0x1E0; offset <= 0x20C; offset += 4)
    {
        g_initializationVisible =
            g_initializationVisible &&
            ReadU32(offset) == 0;
    }

    *reinterpret_cast<fable_u32*>(g_object + 0x19C) = 0xDEADBEEF;
}

int main()
{
    fable_u8 storage[sizeof(NGameText::CDataBank)];
    memset(storage, 0xA5, sizeof(storage));
    g_object = storage;
    g_eventCount = 0;
    g_argumentsValid = true;
    g_initializationVisible = false;

    NGameText::CDataBank* bank =
        new (storage) NGameText::CDataBank;

    static const Event expected[3] =
    {
        kBankFileConstruct,
        kResourceBankConstruct,
        kSetMaximumMemory
    };
    const bool passed =
        sizeof(NGameText::CDataBank) == 0x214 &&
        bank == reinterpret_cast<NGameText::CDataBank*>(storage) &&
        g_eventCount == 3 &&
        memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_argumentsValid &&
        g_initializationVisible &&
        ReadU32(0x19C) == 0;

    if (!passed)
    {
        printf("FABLETLC_GAME_TEXT_DATA_BANK_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_GAME_TEXT_DATA_BANK_CONSTRUCTOR PASS\n");
    return 0;
}
