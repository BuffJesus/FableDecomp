#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_window_title.h"

extern "C" fable_u8 g_FableEmptyCharString_0129AAF4 = 0;
extern "C" fable_u8 g_FableWindowTitleValue_0122D820 = 0;

namespace
{
    const wchar_t kFallbackTitle[] = L"Fable - The Lost Chapters";
    const wchar_t kLocalizedTitle[] = L"Fable: Localized";
    const char kWindowTitleTag[] = "TEXT_GUI_WINDOW_TITLE";
    const char kFontBankName[] = "TEXT_ENGLISH_MAIN";

    CWideStringData g_fallbackWideData = {
        const_cast<wchar_t*>(kFallbackTitle), 0, 0, 1
    };
    CWideStringData g_localizedWideData = {
        const_cast<wchar_t*>(kLocalizedTitle), 0, 0, 1
    };
    CCharStringData g_tagData = {
        const_cast<char*>(kWindowTitleTag),
        sizeof(kWindowTitleTag) - 1,
        0,
        0,
        1
    };
    CCharStringData g_fontBankData = {
        const_cast<char*>(kFontBankName),
        sizeof(kFontBankName) - 1,
        0,
        0,
        1
    };

    struct FakeBank
    {
        void** vtable;
    };

    struct FakeGroup
    {
        void** vtable;
        long references;
    };

    void* g_bankVTable[2];
    void* g_groupVTable[2];
    FakeGroup g_group;
    fable_u32 g_lookupResult;
    bool g_returnGroup;
    fable_u32 g_allocations;
    fable_u32 g_bankConstructs;
    fable_u32 g_bankOpens;
    fable_u32 g_bankDeletes;
    fable_u32 g_crcCalls;
    fable_u32 g_lookupCalls;
    fable_u32 g_groupCalls;
    fable_u32 g_groupReleases;
    fable_u32 g_bankHandleCalls;
    fable_u32 g_wideAssignments;
    fable_u32 g_resultAssignments;
    fable_u32 g_charDestructs;
    fable_u32 g_wideDestructs;
    bool g_crcInputMatched;

    void*& Storage(CWideString& text)
    {
        return *reinterpret_cast<void**>(&text);
    }

    void*& Storage(CCharString& text)
    {
        return *reinterpret_cast<void**>(&text);
    }

    void Reset(bool localized)
    {
        g_lookupResult = localized ? 7 : 0;
        g_returnGroup = localized;
        g_allocations = 0;
        g_bankConstructs = 0;
        g_bankOpens = 0;
        g_bankDeletes = 0;
        g_crcCalls = 0;
        g_lookupCalls = 0;
        g_groupCalls = 0;
        g_groupReleases = 0;
        g_bankHandleCalls = 0;
        g_wideAssignments = 0;
        g_resultAssignments = 0;
        g_charDestructs = 0;
        g_wideDestructs = 0;
        g_crcInputMatched = false;
        g_group.vtable = g_groupVTable;
        g_group.references = 0;
    }

    void __fastcall DeleteBank(void* bank, void*, fable_u32 flags)
    {
        ++g_bankDeletes;
        if ((flags & 1) != 0)
        {
            ::operator delete(bank);
        }
    }

    void __fastcall OpenBank(
        void*,
        void*,
        const CCharString*,
        fable_u32 mode)
    {
        ++g_bankOpens;
        if (mode != 0x100)
        {
            g_bankOpens = 0xFFFFFFFF;
        }
    }

    void __fastcall ReleaseGroup(void*, void*)
    {
        ++g_groupReleases;
    }

    bool CheckFallback()
    {
        Reset(false);
        CWideString title = GetWindowTitle();
        return
            wcscmp(static_cast<const wchar_t*>(title), kFallbackTitle) == 0 &&
            g_allocations == 1 &&
            g_bankConstructs == 1 &&
            g_bankOpens == 1 &&
            g_bankDeletes == 1 &&
            g_crcCalls == 1 &&
            g_crcInputMatched &&
            g_lookupCalls == 1 &&
            g_groupCalls == 0 &&
            g_bankHandleCalls == 0 &&
            g_wideAssignments == 0 &&
            g_resultAssignments == 1 &&
            g_charDestructs == 2 &&
            g_wideDestructs == 1;
    }

    bool CheckLocalized()
    {
        Reset(true);
        CWideString title = GetWindowTitle();
        return
            wcscmp(static_cast<const wchar_t*>(title), kLocalizedTitle) == 0 &&
            g_allocations == 1 &&
            g_bankOpens == 1 &&
            g_bankDeletes == 1 &&
            g_crcCalls == 1 &&
            g_crcInputMatched &&
            g_lookupCalls == 1 &&
            g_groupCalls == 1 &&
            g_group.references == 0 &&
            g_groupReleases == 1 &&
            g_bankHandleCalls == 1 &&
            g_wideAssignments == 1 &&
            g_resultAssignments == 1 &&
            g_charDestructs == 2 &&
            g_wideDestructs == 2;
    }
}

CWideString::CWideString()
    : storage_(0)
{
}

CWideString::~CWideString()
{
}

CWideString::operator const wchar_t*() const
{
    return storage_ != 0 ? storage_->text : L"";
}

extern "C" void __fastcall
FableWindowTitleWideLiteralCtor_004023f0(
    void* destination,
    void*,
    const wchar_t*)
{
    *reinterpret_cast<void**>(destination) = &g_fallbackWideData;
}

extern "C" void __fastcall
FableWindowTitleCharLiteralCtor_004023f0(
    void* destination,
    void*,
    const char*,
    long)
{
    *reinterpret_cast<void**>(destination) = &g_tagData;
}

extern "C" void* __cdecl
FableWindowTitleOperatorNew_004023f0(fable_u32 size)
{
    ++g_allocations;
    return ::operator new(size);
}

extern "C" void* __fastcall
FableWindowTitleBankCtor_004023f0(void* destination, void*)
{
    ++g_bankConstructs;
    reinterpret_cast<FakeBank*>(destination)->vtable = g_bankVTable;
    return destination;
}

extern "C" void* __fastcall
FableWindowTitleGetFontBankName_004023f0(void* destination, void*)
{
    *reinterpret_cast<void**>(destination) = &g_fontBankData;
    return destination;
}

extern "C" void __fastcall
FableWindowTitleCharDtor_004023f0(void*, void*)
{
    ++g_charDestructs;
}

extern "C" fable_u32 __fastcall
FableWindowTitleCrc_004023f0(
    fable_u32 initial,
    const void* data,
    fable_u32 size)
{
    ++g_crcCalls;
    g_crcInputMatched =
        initial == 0 &&
        size == sizeof(kWindowTitleTag) - 1 &&
        memcmp(data, kWindowTitleTag, size) == 0;
    return 0x12345678;
}

extern "C" fable_u32 __fastcall
FableWindowTitleLookup_004023f0(void*, void*, fable_u32 key)
{
    ++g_lookupCalls;
    return key == 0x12345678 ? g_lookupResult : 0;
}

extern "C" void __fastcall
FableWindowTitleGetGroupEntry_004023f0(
    void*,
    void*,
    void* destination,
    fable_u32 index,
    bool load)
{
    ++g_groupCalls;
    FakeGroup* group =
        g_returnGroup && index == g_lookupResult && load ? &g_group : 0;
    *reinterpret_cast<FakeGroup**>(destination) = group;
    if (group != 0)
    {
        ++group->references;
    }
}

extern "C" void* __fastcall
FableWindowTitleGetBankHandle_004023f0(
    void*,
    void* output,
    void*)
{
    ++g_bankHandleCalls;
    *reinterpret_cast<void**>(output) = &g_localizedWideData;
    return output;
}

extern "C" void* __fastcall
FableWindowTitleWideAssign_004023f0(
    void* destination,
    void*,
    const void* source)
{
    ++g_wideAssignments;
    *reinterpret_cast<void**>(destination) =
        *reinterpret_cast<void* const*>(source);
    return destination;
}

extern "C" void __fastcall
FableWindowTitleWideDtor_004023f0(void*, void*)
{
    ++g_wideDestructs;
}

extern "C" void* __fastcall
FableWindowTitleAssignResult_004023f0(
    void* destination,
    const void* source,
    const void*)
{
    ++g_resultAssignments;
    *reinterpret_cast<void**>(destination) =
        *reinterpret_cast<void* const*>(source);
    return destination;
}

int main()
{
    g_bankVTable[0] = reinterpret_cast<void*>(&DeleteBank);
    g_bankVTable[1] = reinterpret_cast<void*>(&OpenBank);
    g_groupVTable[0] = 0;
    g_groupVTable[1] = reinterpret_cast<void*>(&ReleaseGroup);

    if (!CheckFallback() || !CheckLocalized())
    {
        puts("FABLETLC_GET_WINDOW_TITLE_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_GET_WINDOW_TITLE_BEHAVIOR PASS");
    return 0;
}
