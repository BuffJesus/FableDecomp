#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"

namespace
{
    struct ResourceList
    {
        fable_u8 unknown00[0x10];
        void* nextResource;
        fable_u8 unknown14[0x18];
        fable_u32 currentFrame;
    };

    struct Resource
    {
        fable_u32 vtable;
        fable_u32 refCount;
        ResourceList* list;
        fable_u8 unknown0C[0x0C];
        fable_u32 lastUsedFrame;
        fable_u8 unknown1C[0x1C];
        void* asyncData;
    };

    struct BufferedEntry
    {
        Resource* resource;
    };

    enum Event
    {
        kGetType,
        kPrepare,
        kIsFinished,
        kBeginLoadingText,
        kBeginLoading,
        kBuildGroup,
        kMakeFreeSpace,
        kListRemove,
        kListInsert
    };

    fable_u8 g_bank[0x214];
    BufferedEntry g_entry;
    BufferedEntry* g_entries[4];
    Resource g_resource;
    ResourceList g_resourceList;
    Event g_events[16];
    unsigned int g_eventCount;
    fable_u32 g_dataType;
    bool g_asyncFinished;
    bool g_buildCreatesResource;
    bool g_argumentsValid;

    void Record(Event event)
    {
        if (g_eventCount < 16)
        {
            g_events[g_eventCount] = event;
        }
        ++g_eventCount;
    }

    NGameText::CDataBank* Bank()
    {
        return reinterpret_cast<NGameText::CDataBank*>(g_bank);
    }

    void Reset()
    {
        memset(g_bank, 0, sizeof(g_bank));
        memset(&g_entry, 0, sizeof(g_entry));
        memset(&g_resource, 0, sizeof(g_resource));
        memset(&g_resourceList, 0, sizeof(g_resourceList));
        memset(g_entries, 0, sizeof(g_entries));
        *reinterpret_cast<BufferedEntry***>(g_bank + 0x1E0) = g_entries;
        g_entries[2] = &g_entry;
        g_eventCount = 0;
        g_dataType = 0;
        g_asyncFinished = true;
        g_buildCreatesResource = false;
        g_argumentsValid = true;
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
        CIVCountedPointer<const NGameText::CGroupEntry> result =
            Bank()->GetPGroupEntry(2, true);
        const Event expected[] = {kGetType};
        return
            result.data_ == 0 &&
            HasEvents(expected, 1);
    }

    bool CheckPendingNonBlocking()
    {
        Reset();
        g_entry.resource = &g_resource;
        g_resource.asyncData = &g_resource;
        g_asyncFinished = false;
        CIVCountedPointer<const NGameText::CGroupEntry> result =
            Bank()->GetPGroupEntry(2, false);
        const Event expected[] =
        {
            kGetType,
            kPrepare,
            kIsFinished,
            kBeginLoadingText
        };
        return
            result.data_ == 0 &&
            g_resource.refCount == 0 &&
            HasEvents(expected, 4);
    }

    bool CheckSynchronousBuild()
    {
        Reset();
        g_buildCreatesResource = true;
        CIVCountedPointer<const NGameText::CGroupEntry> result =
            Bank()->GetPGroupEntry(2, true);
        const Event expected[] =
        {
            kGetType,
            kPrepare,
            kGetType,
            kBeginLoading,
            kBuildGroup,
            kMakeFreeSpace
        };
        return
            result.data_ ==
                reinterpret_cast<const NGameText::CGroupEntry*>(&g_resource) &&
            g_resource.refCount == 1 &&
            HasEvents(expected, 6);
    }

    bool CheckExistingResourceRefresh()
    {
        Reset();
        g_entry.resource = &g_resource;
        g_resource.asyncData = &g_resource;
        g_resource.list = &g_resourceList;
        g_resource.lastUsedFrame = 3;
        g_resourceList.currentFrame = 9;
        g_resourceList.nextResource = &g_entry;
        CIVCountedPointer<const NGameText::CGroupEntry> result =
            Bank()->GetPGroupEntry(2, true);
        const Event expected[] =
        {
            kGetType,
            kPrepare,
            kIsFinished,
            kBuildGroup,
            kListRemove,
            kListInsert
        };
        return
            result.data_ ==
                reinterpret_cast<const NGameText::CGroupEntry*>(&g_resource) &&
            g_resource.refCount == 1 &&
            g_resource.lastUsedFrame == 9 &&
            HasEvents(expected, 6);
    }
}

extern "C" fable_u32 FABLE_FASTCALL
FableGameTextGetEntryDataType_009C9070(
    const void* bank,
    void*,
    fable_u32 index)
{
    Record(kGetType);
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_bank &&
        index == 2;
    return g_dataType;
}

extern "C" void FABLE_FASTCALL
FableGameTextPrepareEntry_009C9070(
    void* bank,
    void*)
{
    Record(kPrepare);
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_bank;
}

extern "C" bool FABLE_FASTCALL
FableGameTextAsyncIsFinished_009C9070(
    void* asyncData,
    void*)
{
    Record(kIsFinished);
    g_argumentsValid =
        g_argumentsValid &&
        asyncData == &g_resource;
    return g_asyncFinished;
}

extern "C" void FABLE_FASTCALL
FableGameTextBeginLoadingText_009C9070(
    void* bank,
    void*,
    fable_u32 index)
{
    Record(kBeginLoadingText);
    g_argumentsValid =
        g_argumentsValid &&
        bank == g_bank &&
        index == 2;
}

extern "C" void FABLE_FASTCALL
FableGameTextGroupBeginLoading_009C9070(
    void* groupEntry,
    void*,
    fable_u32 index,
    void* bank)
{
    Record(kBeginLoading);
    g_argumentsValid =
        g_argumentsValid &&
        groupEntry == &g_entry &&
        index == 2 &&
        bank == g_bank;
}

extern "C" void FABLE_FASTCALL
FableGameTextBuildGroup_009C9070(
    void* groupEntry,
    void*)
{
    Record(kBuildGroup);
    g_argumentsValid =
        g_argumentsValid &&
        groupEntry == &g_entry;
    if (g_buildCreatesResource)
    {
        g_entry.resource = &g_resource;
    }
}

extern "C" void FABLE_FASTCALL
FableGameTextMakeFreeSpace_009C9070(
    void* resourceList,
    void*,
    fable_u32 force)
{
    Record(kMakeFreeSpace);
    g_argumentsValid =
        g_argumentsValid &&
        resourceList == g_bank + 0x168 &&
        force == 0;
}

extern "C" void FABLE_FASTCALL
FableGameTextResourceListRemove_009C9070(
    void* resource,
    void*)
{
    Record(kListRemove);
    g_argumentsValid =
        g_argumentsValid &&
        resource == &g_resource;
}

extern "C" void FABLE_FASTCALL
FableGameTextResourceListInsert_009C9070(
    void* resource,
    void*,
    void* nextResource,
    void* headResource)
{
    Record(kListInsert);
    g_argumentsValid =
        g_argumentsValid &&
        resource == &g_resource &&
        nextResource == g_resourceList.nextResource &&
        headResource ==
            reinterpret_cast<fable_u8*>(&g_resourceList) + 4;
}

int main()
{
    const bool passed =
        CheckWrongType() &&
        CheckPendingNonBlocking() &&
        CheckSynchronousBuild() &&
        CheckExistingResourceRefresh() &&
        g_argumentsValid;
    if (!passed)
    {
        printf("FABLETLC_GAME_TEXT_GET_GROUP_ENTRY FAIL\n");
        return 1;
    }

    printf("FABLETLC_GAME_TEXT_GET_GROUP_ENTRY PASS\n");
    return 0;
}
