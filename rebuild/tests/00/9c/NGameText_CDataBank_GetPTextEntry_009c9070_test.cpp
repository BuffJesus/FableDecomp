#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"
#include "test_support/resource_list_stubs.h"

using NGameText::CDataBank;
using NGameText::CDataBankEntry;
using NGameText::CDataBankTextEntry;
using NGameText::CTextEntry;

namespace
{
    enum Event
    {
        kGetType,
        kAsyncUpdate,
        kIsFinished,
        kBeginLoadingText,
        kBeginLoading,
        kBuildText,
        kMakeFreeSpace,
        kListRemove,
        kListInsert
    };

    const unsigned long kIndex = 2;

    fable_u8 g_bank[sizeof(CDataBank)];
    CDataBankTextEntry g_entry;
    CDataBankEntry* g_entries[4];
    CTextEntry g_text;
    CResourceList g_list;
    // A layout view: no I/O request or bank reference is constructed by this fixture.
    unsigned long g_asyncBytes[sizeof(CBankFileAsyncData)/4];
    CBankFileAsyncData& g_asyncData=*reinterpret_cast<CBankFileAsyncData*>(g_asyncBytes);
    Event g_events[16];
    unsigned int g_eventCount;
    unsigned long g_dataType;
    bool g_asyncFinished;
    bool g_buildCreatesText;
    bool g_argumentsValid = true;

    void Record(Event event)
    {
        if (g_eventCount < 16)
            g_events[g_eventCount] = event;
        ++g_eventCount;
    }

    CDataBank* Bank()
    {
        return reinterpret_cast<CDataBank*>(g_bank);
    }

    void Reset()
    {
        memset(g_bank, 0, sizeof(g_bank));
        memset(g_entries, 0, sizeof(g_entries));
        Bank()->Entries._M_start = g_entries;
        g_entries[kIndex] = &g_entry;
        g_entry.PText.data_ = 0;
        g_text.IVRefCount = 1;
        g_text.ResourceList = 0;
        g_text.PrevResource = 0;
        g_text.NextResource = 0;
        g_text.LastUsedFrame = 0;
        g_text.PFileData.Data = 0;
        g_text.PFileData.Info = 0;
        g_list.CurrentFrame = 0;
        g_list.Head.PrevResource = 0;
        g_eventCount = 0;
        g_dataType = 0;
        g_asyncFinished = true;
        g_buildCreatesText = false;
    }

    bool HasEvents(const Event* expected, unsigned int count)
    {
        return
            g_eventCount == count &&
            memcmp(g_events, expected, count * sizeof(Event)) == 0;
    }

    bool CheckWrongType()
    {
        Reset();
        g_dataType = 1;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, true);
        const Event expected[] = {kGetType};
        return result.IsNull() && HasEvents(expected, 1);
    }

    bool CheckMissingEntry()
    {
        Reset();
        g_entries[kIndex] = 0;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, true);
        const Event expected[] = {kGetType};
        return result.IsNull() && HasEvents(expected, 1);
    }

    bool CheckPendingStartsLoad()
    {
        Reset();
        g_entry.PText.data_ = &g_text;
        g_text.PFileData.Data = &g_asyncData;
        g_asyncFinished = false;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kIsFinished,
            kBeginLoadingText
        };
        return
            result.IsNull() &&
            g_text.IVRefCount == 1 &&
            HasEvents(expected, 4);
    }

    bool CheckUnloadedStartsLoad()
    {
        Reset();
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kBeginLoadingText
        };
        return result.IsNull() && HasEvents(expected, 3);
    }

    bool CheckSynchronousBuild()
    {
        Reset();
        g_buildCreatesText = true;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, true);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kGetType,
            kBeginLoading,
            kBuildText,
            kMakeFreeSpace
        };
        return
            result.data_ == &g_text &&
            g_text.IVRefCount == 2 &&
            HasEvents(expected, 6);
    }

    bool CheckFinishedLoadIsBuiltAndMadeMRU()
    {
        Reset();
        g_entry.PText.data_ = &g_text;
        g_text.PFileData.Data = &g_asyncData;
        g_text.ResourceList = &g_list;
        g_text.LastUsedFrame = 3;
        g_list.CurrentFrame = 9;
        g_list.Head.PrevResource = &g_list.Head;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kIsFinished,
            kBuildText,
            kListRemove,
            kListInsert
        };
        return
            result.data_ == &g_text &&
            g_text.IVRefCount == 2 &&
            g_text.LastUsedFrame == 9 &&
            HasEvents(expected, 6);
    }

    bool CheckLoadedTextIsShared()
    {
        Reset();
        g_entry.PText.data_ = &g_text;
        g_text.ResourceList = &g_list;
        g_text.LastUsedFrame = 5;
        g_list.CurrentFrame = 5;
        CIVCountedPointer<const CTextEntry> result =
            Bank()->GetPTextEntry(kIndex, false);
        const Event expected[] = {kGetType, kAsyncUpdate};
        return
            result.data_ == &g_text &&
            g_text.IVRefCount == 2 &&
            HasEvents(expected, 2);
    }
}

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const
{
    g_argumentsValid = false;
}

unsigned long CBankFile::GetEntryDataType(unsigned long index) const
{
    Record(kGetType);
    g_argumentsValid =
        g_argumentsValid &&
        this == static_cast<const CBankFile*>(Bank()) &&
        index == kIndex;
    return g_dataType;
}

void CBankFileAsync::AsyncUpdatePending()
{
    Record(kAsyncUpdate);
    g_argumentsValid =
        g_argumentsValid &&
        this == static_cast<CBankFile*>(Bank());
}

bool CBankFileAsyncData::IsFinished() const
{
    Record(kIsFinished);
    g_argumentsValid = g_argumentsValid && this == &g_asyncData;
    return g_asyncFinished;
}

void CDataBank::BeginLoadingText(unsigned long index) const
{
    Record(kBeginLoadingText);
    g_argumentsValid =
        g_argumentsValid &&
        this == Bank() &&
        index == kIndex;
}

void CDataBankTextEntry::BeginLoading(
    unsigned long index,
    CDataBank& bank) const
{
    Record(kBeginLoading);
    g_argumentsValid =
        g_argumentsValid &&
        this == &g_entry &&
        index == kIndex &&
        &bank == Bank();
}

void CDataBankTextEntry::BuildText() const
{
    Record(kBuildText);
    g_argumentsValid = g_argumentsValid && this == &g_entry;
    if (g_buildCreatesText)
        g_entry.PText.data_ = &g_text;
}

bool CResourceList::MakeFreeSpace(unsigned long size)
{
    Record(kMakeFreeSpace);
    g_argumentsValid =
        g_argumentsValid &&
        this == &Bank()->ResourceList &&
        reinterpret_cast<fable_u8*>(this) == g_bank + 0x168 &&
        size == 0;
    return true;
}

void CResource::ListRemove()
{
    Record(kListRemove);
    g_argumentsValid = g_argumentsValid && this == &g_text;
}

void CResource::ListInsert(CResource* prev, CResource* next)
{
    Record(kListInsert);
    g_argumentsValid =
        g_argumentsValid &&
        this == &g_text &&
        prev == g_list.Head.PrevResource &&
        next == &g_list.Head;
}

int main()
{
    const bool passed =
        CheckWrongType() &&
        CheckMissingEntry() &&
        CheckPendingStartsLoad() &&
        CheckUnloadedStartsLoad() &&
        CheckSynchronousBuild() &&
        CheckFinishedLoadIsBuiltAndMadeMRU() &&
        CheckLoadedTextIsShared() &&
        g_argumentsValid;
    // Borrowed static resources must be detached before static teardown.
    g_entry.PText.data_ = 0;
    g_text.ResourceList = 0;
    if (!passed)
    {
        printf("FABLETLC_GAME_TEXT_GET_TEXT_ENTRY FAIL\n");
        return 1;
    }

    printf("FABLETLC_GAME_TEXT_GET_TEXT_ENTRY PASS\n");
    return 0;
}

CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}

#include "test_support/text_resource_unload_native.h"
