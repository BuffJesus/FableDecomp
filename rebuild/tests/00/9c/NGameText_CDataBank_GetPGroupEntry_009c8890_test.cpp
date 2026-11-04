#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"
#include "test_support/resource_list_stubs.h"

using NGameText::CDataBank;
using NGameText::CDataBankEntry;
using NGameText::CDataBankGroupEntry;
using NGameText::CGroupEntry;

namespace
{
    enum Event
    {
        kGetType,
        kAsyncUpdate,
        kIsFinished,
        kBeginLoadingGroup,
        kBeginLoading,
        kBuildGroup,
        kMakeFreeSpace,
        kListRemove,
        kListInsert
    };

    const unsigned long kIndex = 2;

    fable_u8 g_bank[sizeof(CDataBank)];
    CDataBankGroupEntry g_entry;
    CDataBankEntry* g_entries[4];
    CGroupEntry g_group;
    CResourceList g_list;
    // A layout view: no I/O request or bank reference is constructed by this fixture.
    unsigned long g_asyncBytes[sizeof(CBankFileAsyncData)/4];
    CBankFileAsyncData& g_asyncData=*reinterpret_cast<CBankFileAsyncData*>(g_asyncBytes);
    Event g_events[16];
    unsigned int g_eventCount;
    unsigned long g_dataType;
    bool g_asyncFinished;
    bool g_buildCreatesGroup;
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
        g_entry.PGroup.data_ = 0;
        g_group.IVRefCount = 1;
        g_group.ResourceList = 0;
        g_group.PrevResource = 0;
        g_group.NextResource = 0;
        g_group.LastUsedFrame = 0;
        g_group.PFileData.Data = 0;
        g_group.PFileData.Info = 0;
        g_list.CurrentFrame = 0;
        g_list.Head.PrevResource = 0;
        g_eventCount = 0;
        g_dataType = 1;
        g_asyncFinished = true;
        g_buildCreatesGroup = false;
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
        g_dataType = 0;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, true);
        const Event expected[] = {kGetType};
        return result.IsNull() && HasEvents(expected, 1);
    }

    bool CheckMissingEntry()
    {
        Reset();
        g_entries[kIndex] = 0;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, true);
        const Event expected[] = {kGetType};
        return result.IsNull() && HasEvents(expected, 1);
    }

    bool CheckPendingStartsLoad()
    {
        Reset();
        g_entry.PGroup.data_ = &g_group;
        g_group.PFileData.Data = &g_asyncData;
        g_asyncFinished = false;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kIsFinished,
            kBeginLoadingGroup
        };
        return
            result.IsNull() &&
            g_group.IVRefCount == 1 &&
            HasEvents(expected, 4);
    }

    bool CheckUnloadedStartsLoad()
    {
        Reset();
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kBeginLoadingGroup
        };
        return result.IsNull() && HasEvents(expected, 3);
    }

    bool CheckSynchronousBuild()
    {
        Reset();
        g_buildCreatesGroup = true;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, true);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kGetType,
            kBeginLoading,
            kBuildGroup,
            kMakeFreeSpace
        };
        return
            result.data_ == &g_group &&
            g_group.IVRefCount == 2 &&
            HasEvents(expected, 6);
    }

    bool CheckFinishedLoadIsBuiltAndMadeMRU()
    {
        Reset();
        g_entry.PGroup.data_ = &g_group;
        g_group.PFileData.Data = &g_asyncData;
        g_group.ResourceList = &g_list;
        g_group.LastUsedFrame = 3;
        g_list.CurrentFrame = 9;
        g_list.Head.PrevResource = &g_list.Head;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, false);
        const Event expected[] =
        {
            kGetType,
            kAsyncUpdate,
            kIsFinished,
            kBuildGroup,
            kListRemove,
            kListInsert
        };
        return
            result.data_ == &g_group &&
            g_group.IVRefCount == 2 &&
            g_group.LastUsedFrame == 9 &&
            HasEvents(expected, 6);
    }

    bool CheckLoadedGroupIsShared()
    {
        Reset();
        g_entry.PGroup.data_ = &g_group;
        g_group.ResourceList = &g_list;
        g_group.LastUsedFrame = 5;
        g_list.CurrentFrame = 5;
        CIVCountedPointer<const CGroupEntry> result =
            Bank()->GetPGroupEntry(kIndex, false);
        const Event expected[] = {kGetType, kAsyncUpdate};
        return
            result.data_ == &g_group &&
            g_group.IVRefCount == 2 &&
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

void CDataBank::BeginLoadingGroup(unsigned long index) const
{
    Record(kBeginLoadingGroup);
    g_argumentsValid =
        g_argumentsValid &&
        this == Bank() &&
        index == kIndex;
}

void CDataBankGroupEntry::BeginLoading(
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

void CDataBankGroupEntry::BuildGroup() const
{
    Record(kBuildGroup);
    g_argumentsValid = g_argumentsValid && this == &g_entry;
    if (g_buildCreatesGroup)
        g_entry.PGroup.data_ = &g_group;
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
    g_argumentsValid = g_argumentsValid && this == &g_group;
}

void CResource::ListInsert(CResource* prev, CResource* next)
{
    Record(kListInsert);
    g_argumentsValid =
        g_argumentsValid &&
        this == &g_group &&
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
        CheckLoadedGroupIsShared() &&
        g_argumentsValid;
    // Borrowed static resources must be detached before static teardown.
    g_entry.PGroup.data_ = 0;
    g_group.ResourceList = 0;
    if (!passed)
    {
        printf("FABLETLC_GAME_TEXT_GET_GROUP_ENTRY FAIL\n");
        return 1;
    }

    printf("FABLETLC_GAME_TEXT_GET_GROUP_ENTRY PASS\n");
    return 0;
}

#include "test_support/text_resource_unload_native.h"
