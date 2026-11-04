#include "test_support/async_bank_lifetime_stubs.h"
#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"
#include "test_support/bank_file_member_stubs.h"
#include "test_support/resource_list_stubs.h"
#include "test_support/game_text_unused_virtual_stubs.h"

namespace
{
    enum Event
    {
        kAsyncBankFileConstruct,
        kResourceBankConstruct,
        kSetMaximumMemory
    };

    fable_u8* g_object;
    Event g_events[3];
    unsigned int g_eventCount;
    bool g_argumentsValid;
    bool g_initializationVisible;
    void* g_bankFileVTable;
    void* g_resourceBankVTable;

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

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const
{
}

CResourceBank::CResourceBank()
{
    Record(kResourceBankConstruct);
    g_argumentsValid =
        g_argumentsValid &&
        reinterpret_cast<fable_u8*>(this) == g_object + 0x164;
    memset(&ResourceList.Head, 0x5A, sizeof(ResourceList.Head));
    ResourceList.UnloadDelay = 0x5A5A5A5A;
    g_resourceBankVTable = *reinterpret_cast<void**>(this);
}

CResourceBank::~CResourceBank()
{
}

CBankFile::CBankFile()
{
}

CBankFileAsync::CBankFileAsync()
{
    Record(kAsyncBankFileConstruct);
    g_argumentsValid =
        g_argumentsValid &&
        reinterpret_cast<fable_u8*>(this) == g_object;
    g_bankFileVTable = *reinterpret_cast<void**>(this);
}

CBankFile::~CBankFile()
{
}

template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list()
{
}

CBankFileAsync::~CBankFileAsync()
{
}

bool CBankFile::OpenReadOnly(const CCharString&, unsigned long)
{
    return false;
}

NGameText::CDataBank::~CDataBank()
{
}

void CResourceList::SetMaximumMemory(long maximumMemory)
{
    Record(kSetMaximumMemory);
    g_argumentsValid =
        g_argumentsValid &&
        reinterpret_cast<fable_u8*>(this) == g_object + 0x168 &&
        maximumMemory == 0x10000;

    // The derived vtables and zeroed members must be in place before the call.
    g_initializationVisible =
        ReadU32(0x000) != 0xA5A5A5A5 &&
        ReadU32(0x000) != reinterpret_cast<fable_u32>(g_bankFileVTable) &&
        ReadU32(0x164) != reinterpret_cast<fable_u32>(g_resourceBankVTable) &&
        ReadU32(0x210) == 0;
    for (unsigned int offset = 0x1E0; offset <= 0x20C; offset += 4)
    {
        g_initializationVisible =
            g_initializationVisible &&
            ReadU32(offset) == 0;
    }

    MaximumMemory = maximumMemory;
    UnloadDelay = 0xDEADBEEF;
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
        kAsyncBankFileConstruct,
        kResourceBankConstruct,
        kSetMaximumMemory
    };
    const bool passed =
        sizeof(NGameText::CDataBank) == 0x214 &&
        bank == reinterpret_cast<NGameText::CDataBank*>(storage) &&
        static_cast<CResourceBank*>(bank) ==
            reinterpret_cast<CResourceBank*>(storage + 0x164) &&
        g_eventCount == 3 &&
        memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_argumentsValid &&
        g_initializationVisible &&
        ReadU32(0x168 + 0x28) == 0x10000 &&
        ReadU32(0x19C) == 0;

    if (!passed)
    {
        printf("FABLETLC_GAME_TEXT_DATA_BANK_CONSTRUCTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_GAME_TEXT_DATA_BANK_CONSTRUCTOR PASS\n");
    return 0;
}

// Unopened bank boundary for this isolated fixture.
void NGameText::CDataBank::Close() { CBankFile::Close(); }
