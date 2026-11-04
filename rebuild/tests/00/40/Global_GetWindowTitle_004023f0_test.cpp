#include "test_support/async_bank_lifetime_stubs.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_window_title.h"
#include "fable_game_text_bank.h"
#include "fable_language.h"
#include "test_support/resource_list_stubs.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "test_support/bank_file_cleanup_stubs.h"
#include "test_support/game_text_unused_virtual_stubs.h"

// Recorders for the real callees. A CWideString's storage word holds its text
// pointer; a CCharString's points at a CCharStringData from a small pool.
char g_FableEmptyNarrowString_0129AAF4[1] = {0};

namespace
{
    const wchar_t kFallbackTitle[] = L"Fable - The Lost Chapters";
    const wchar_t kLocalizedTitle[] = L"Fable: Localized";
    const char kWindowTitleSymbol[] = "TEXT_GUI_WINDOW_TITLE";
    const char kFontBankName[] = "TEXT_ENGLISH_MAIN";
    const unsigned long kSymbolCRC = 0x12345678;

    CCharStringData g_charData[4];
    unsigned int g_charDataUsed;
    wchar_t g_appendStorage[64];

    NGameText::CTextEntry g_entry;
    unsigned long g_foundIndex;
    bool g_entryExists;

    unsigned int g_bankConstructs;
    unsigned int g_bankDeletes;
    unsigned int g_bankOpens;
    bool g_openArgumentsMatched;
    unsigned int g_crcCalls;
    bool g_crcInputMatched;
    unsigned int g_lookups;
    unsigned int g_entryCalls;
    bool g_entryArgumentsMatched;
    unsigned int g_getTextCalls;
    unsigned int g_entryDeletes;
    unsigned int g_charDestructs;
    unsigned int g_wideDestructs;
    const wchar_t* g_appendLeft;
    const wchar_t* g_appendRight;

    const wchar_t*& Text(CWideString& value)
    {
        return *reinterpret_cast<const wchar_t**>(&value);
    }

    const wchar_t* Text(const CWideString& value)
    {
        return *reinterpret_cast<const wchar_t* const*>(&value);
    }

    const CCharStringData* Data(const CCharString& value)
    {
        return *reinterpret_cast<const CCharStringData* const*>(&value);
    }

    void Reset(unsigned long foundIndex, bool entryExists)
    {
        g_charDataUsed = 0;
        g_foundIndex = foundIndex;
        g_entryExists = entryExists;
        g_entry.IVRefCount = 0;
        g_bankConstructs = 0;
        g_bankDeletes = 0;
        g_bankOpens = 0;
        g_openArgumentsMatched = false;
        g_crcCalls = 0;
        g_crcInputMatched = false;
        g_lookups = 0;
        g_entryCalls = 0;
        g_entryArgumentsMatched = false;
        g_getTextCalls = 0;
        g_entryDeletes = 0;
        g_charDestructs = 0;
        g_wideDestructs = 0;
        g_appendLeft = 0;
        g_appendRight = 0;
    }
}

CWideString::CWideString(const wchar_t* text)
{
    Text(*this) = text;
}

CWideString::CWideString()
{
    Text(*this) = 0;
}

CWideString::~CWideString()
{
    ++g_wideDestructs;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    Text(*this) = Text(other);
    return *this;
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    g_appendLeft = Text(left);
    g_appendRight = right;
    wcscpy(g_appendStorage, Text(left));
    wcscat(g_appendStorage, right);
    return CWideString(g_appendStorage);
}

CCharString::CCharString(const char* text, long length)
{
    CCharStringData& data = g_charData[g_charDataUsed++];
    data.text = const_cast<char*>(text);
    data.unknown04 =
        static_cast<fable_u32>(length < 0 ? strlen(text) : length);
    *reinterpret_cast<CCharStringData**>(this) = &data;
}

CCharString::CCharString()
{
    *reinterpret_cast<CCharStringData**>(this) = 0;
}

CCharString::~CCharString()
{
    ++g_charDestructs;
}

CCharString FABLE_FASTCALL NLocalisation::GetTextBankName()
{
    return CCharString(kFontBankName, -1);
}

fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount)
{
    ++g_crcCalls;
    g_crcInputMatched =
        seed == 0 &&
        byteCount == sizeof(kWindowTitleSymbol) - 1 &&
        memcmp(data, kWindowTitleSymbol, byteCount) == 0;
    return kSymbolCRC;
}

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const
{
    ++g_entryDeletes;
}

CWideString NGameText::CTextEntry::GetText() const
{
    ++g_getTextCalls;
    return CWideString(kLocalizedTitle);
}

CResourceBank::CResourceBank()
{
}

CResourceBank::~CResourceBank()
{
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CPackedUIntArray::CPackedUIntArray()
    : PackedInts(0), Size(0), Bits(0), Bias(0) {}

CBankFile::CBankFile()
{
}

CBankFile::~CBankFile()
{
}

template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list()
{
}

CBankFileAsync::CBankFileAsync()
{
}

CBankFileAsync::~CBankFileAsync()
{
}

bool CBankFile::OpenReadOnly(
    const CCharString& bankHandle,
    unsigned long openFlags)
{
    ++g_bankOpens;
    const CCharStringData* data = Data(bankHandle);
    g_openArgumentsMatched =
        openFlags == 0x100 &&
        data != 0 &&
        strcmp(data->text, kFontBankName) == 0;
    return true;
}

unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long crc) const
{
    ++g_lookups;
    return crc == kSymbolCRC ? g_foundIndex : 0;
}

NGameText::CDataBank::CDataBank()
{
    ++g_bankConstructs;
}

NGameText::CDataBank::~CDataBank()
{
    ++g_bankDeletes;
}

CIVCountedPointer<const NGameText::CTextEntry>
NGameText::CDataBank::GetPTextEntry(
    unsigned long index,
    bool allowPending) const
{
    ++g_entryCalls;
    g_entryArgumentsMatched = index == g_foundIndex && allowPending;
    CIVCountedPointer<const NGameText::CTextEntry> entry;
    if (g_entryExists)
    {
        entry.data_ = &g_entry;
        ++g_entry.IVRefCount;
    }
    // Return an owning pointer with balanced copy and destruction semantics.
    return entry;
}

namespace
{
    bool CheckCommon(const CWideString& title, const wchar_t* expected)
    {
        wchar_t spaced[64];
        wcscpy(spaced, expected);
        wcscat(spaced, L" ");
        return
            wcscmp(Text(title), spaced) == 0 &&
            wcscmp(g_appendLeft, expected) == 0 &&
            wcscmp(g_appendRight, L" ") == 0 &&
            g_bankConstructs == 1 &&
            g_bankOpens == 1 &&
            g_openArgumentsMatched &&
            g_bankDeletes == 1 &&
            g_crcCalls == 1 &&
            g_crcInputMatched &&
            g_lookups == 1 &&
            // The typed CBankFile now destroys BankHandle and
            // HeaderEnumerationType as well as the two title temporaries.
            g_charDestructs == 4;
    }

    bool CheckSymbolMissing()
    {
        Reset(0, true);
        CWideString title = GetWindowTitle();
        return
            CheckCommon(title, kFallbackTitle) &&
            g_entryCalls == 0 &&
            g_getTextCalls == 0 &&
            g_wideDestructs == 3;
    }

    bool CheckEntryMissing()
    {
        Reset(7, false);
        CWideString title = GetWindowTitle();
        return
            CheckCommon(title, kFallbackTitle) &&
            g_entryCalls == 1 &&
            g_entryArgumentsMatched &&
            g_getTextCalls == 0 &&
            g_wideDestructs == 3;
    }

    bool CheckLocalized()
    {
        Reset(7, true);
        CWideString title = GetWindowTitle();
        return
            CheckCommon(title, kLocalizedTitle) &&
            g_entryCalls == 1 &&
            g_entryArgumentsMatched &&
            g_getTextCalls == 1 &&
            g_entry.IVRefCount == 0 &&
            g_entryDeletes == 1 &&
            g_wideDestructs == 4;
    }
}

int main()
{
    if (!CheckSymbolMissing() || !CheckEntryMissing() || !CheckLocalized())
    {
        puts("FABLETLC_GET_WINDOW_TITLE_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_GET_WINDOW_TITLE_BEHAVIOR PASS");
    return 0;
}

// Unopened bank boundary for this isolated fixture.
void NGameText::CDataBank::Close() { CBankFile::Close(); }

#include "test_support/text_resource_unload_native.h"
// This fixture provides loaded text only; unexpected async access is an error.
bool CBankFileAsyncData::IsFinished() const { abort(); return false; }
