#pragma once

#include <stddef.h>
#include <string.h>

#include "rebuild_abi.h"
#include "fable_stlport_string.h"
#include "fable_basic_string.h"

// Ego_r.pdb: CWideStringData { basic_string<wchar_t> Data; long NoRefs; }.
// Retail's STLport string is 12 bytes, so NoRefs (`owners`) sits at +0x0C.
// CWideString::AllocStringData 0x0099B3C0 news it through the two ctors below
// (both inlined; the C-string one needs __forceinline, as its length loop
// otherwise stops VC7.1 inlining it, cf. CWideString::operator==).
class CWideStringData
{
public:
    __forceinline explicit CWideStringData(const wchar_t* text)
        : Data(text), owners(1)
    {
    }

    CWideStringData(const wchar_t* text, long length)
        : Data(text, length), owners(1)
    {
    }

    CWideStringData(size_t count,wchar_t character) : Data(count,character),owners(1) {}
    _STL::basic_string<wchar_t> Data;
    fable_i32 owners;

    // Field names used by code landed before Data was modelled: the
    // string's start, finish and end of storage. Inline accessors only.
    wchar_t* GetText_() const { return Data._M_start; }
    void SetText_(wchar_t* value) { Data._M_start = value; }
    size_t GetFinish_() const { return reinterpret_cast<size_t>(Data._M_finish); }
    void SetFinish_(size_t value) { Data._M_finish = reinterpret_cast<wchar_t*>(value); }
    size_t GetEndOfStorage_() const { return reinterpret_cast<size_t>(Data._M_end_of_storage._M_data); }
    void SetEndOfStorage_(size_t value) { Data._M_end_of_storage._M_data = reinterpret_cast<wchar_t*>(value); }
    __declspec(property(get = GetText_, put = SetText_)) wchar_t* text;
    __declspec(property(get = GetFinish_, put = SetFinish_)) size_t unknown04;
    __declspec(property(get = GetEndOfStorage_, put = SetEndOfStorage_)) size_t unknown08;
};
FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);

// Ego_r.pdb: CCharStringData { CBasicString<char> Data; long NoRefs; },
// packed to 0x11 bytes (NoRefs, here `owners`, at +0x0D). CCharString::AllocStringData
// 0x0099EA60 news it through the two inline ctors below.
#pragma pack(push, 1)
class CCharStringData
{
public:
    // FableWin ??0CCharStringData@@QAE@PBD@Z; inline in retail.
    explicit CCharStringData(const char* text)
        : Data(text), owners(1)
    {
    }

    // FableWin ??0CCharStringData@@QAE@PBDJ@Z; inline in retail (VC7.1
    // keeps it out of line in AllocStringData unless forced).
    // FableWin ??0CCharStringData@@QAE@JD@Z; inline in retail
    // (CCharString::MakeOwnCopyOfDataOrEmptyString 0x0099EB40).
    __forceinline CCharStringData(long length, char character)
        : Data(length, character), owners(1)
    {
    }

    __forceinline CCharStringData(const char* text, long length)
        : Data(text, length), owners(1)
    {
    }

    // Fixture storage only (see CBasicString()); retail has no default ctor.
    CCharStringData()
    {
    }

    bool operator<(const CCharStringData& other) const;
    // Ego_r ?ReadFromFile@CCharStringData@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z,
    // retail 0x0099E4D0: fill the existing buffer from the file.
    void ReadFromFile(class CAFile& file, class CASuspendableProcess* process, long length);
    // CBasicString<char>::operator==(char*) @ 0x4115A0 on the leading Data.
    __forceinline bool operator==(const char* other) const
    {
        return Data == other;
    }

    CBasicString<char> Data;
    // Ego_r's NoRefs; kept under the name the landed code uses (a property
    // cannot stand in for it: `--data->owners <= 0` needs a real lvalue).
    fable_i32 owners;

    // Field names used by code landed before Data was modelled: the buffer,
    // the length, the raw size dword (31-bit DataLength plus a spare top
    // bit) and the UseFastExtend byte. Inline accessors.
    char* GetText_() const { return Data.PData; }
    void SetText_(char* value) { Data.PData = value; }
    fable_u32 GetLength_() const { return Data.StringLength; }
    void SetLength_(fable_u32 value) { Data.StringLength = value; }
    fable_u32 GetSizeWord_() const { return *reinterpret_cast<const fable_u32*>(reinterpret_cast<const char*>(this) + 8); }
    void SetSizeWord_(fable_u32 value) { *reinterpret_cast<fable_u32*>(reinterpret_cast<char*>(this) + 8) = value; }
    fable_u8 GetFlagsByte_() const { return *(reinterpret_cast<const fable_u8*>(this) + 0x0C); }
    void SetFlagsByte_(fable_u8 value) { *(reinterpret_cast<fable_u8*>(this) + 0x0C) = value; }
    __declspec(property(get = GetText_, put = SetText_)) char* text;
    __declspec(property(get = GetLength_, put = SetLength_)) fable_u32 unknown04;
    __declspec(property(get = GetSizeWord_, put = SetSizeWord_)) fable_u32 unknown08;
    __declspec(property(get = GetFlagsByte_, put = SetFlagsByte_)) fable_u8 flags0C;
};
#pragma pack(pop)

class CCharString;
class CAFile;
class CASuspendableProcess;

int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs,
    const unsigned short* rhs,
    unsigned long count);

class CWideString
{
public:
    unsigned long Sizeof() const;
    CWideString();
    CWideString(const CWideString& other);
    // Not explicit: retail passes literals through implicit conversion temporaries
    // (InitialiseConsoleVariables 0x413c50 only matches written that way).
    CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    long GetByteLength() const;
    CCharString ToCharString() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    // FableWin ??4CWideString@@QAEABV0@_W@Z, retail 0x0099B860 (manifest label UnassignString).
    const CWideString& operator=(wchar_t character);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    // FableWin ??YCWideString@@QAEABV0@_W@Z, retail 0x0099B9A0.
    const CWideString& operator+=(wchar_t character);
    // Ego_r ?Sprintf@CWideString@@QAAXPB_WZZ / ?SprintfDynamic@..., retail 0x0099BA70 /
    // 0x0099BB80 (lib_wide_string.cpp).
    void Sprintf(const wchar_t* format, ...);
    void SprintfDynamic(const wchar_t* format, ...);
    // Ego_r ?SetLength@CWideString@@QAEXJ@Z / ?LoadFromFile@CWideString@@QAEXAAVCAFile@@J@Z,
    // retail 0x0099BC50 / 0x0099BD00.
    // Ego_r ?SetAsSubString@CWideString@@QAEXPB_WJ@Z, retail 0x0099B770.
    void SetAsSubString(const wchar_t* text, long length);
    void SetLength(long length);
    void LoadFromFile(CAFile& file, long length);
    // FableWin ?GetAsCharArray@CWideString@@QBEPB_WXZ, retail 0x0099AF10 (the
    // same body as operator const wchar_t* 0x0099AF00; both are called).
    const wchar_t* GetAsCharArray() const;
    bool operator!=(const wchar_t* other) const;
    // FableWin ??9CWideString@@QBE_NABV0@@Z, retail 0x0099B0F0, over the inline ==.
    bool operator!=(const CWideString& other) const;
    // Shared data is equal; a null side is not; then length and MemCmp_Unsigned16.
    __forceinline bool operator==(const CWideString& other) const
    {
        if (storage_ == other.storage_)
            return true;
        if (storage_ == 0 || other.storage_ == 0)
            return false;
        const wchar_t* otherFirst = other.storage_->text;
        const wchar_t* first = storage_->text;
        unsigned long length = reinterpret_cast<const wchar_t*>(storage_->unknown04) - first;
        unsigned long otherLength = reinterpret_cast<const wchar_t*>(other.storage_->unknown04) - otherFirst;
        return length == otherLength && MemCmp_Unsigned16(first, otherFirst, length) == 0;
    }
    bool operator<(const CWideString& other) const;
    CWideString GetSubString(unsigned long start, unsigned long count) const;
    // FableWin ?FindCharPos@CWideString@@QBEJ_W@Z, retail 0x0099B070 (wcschr; -1 if absent).
    long FindCharPos(wchar_t ch) const;
    // Ego_r lib_wide_string.cpp members, retail 0x0099AF20-0x0099B260 (CWideString_Compare_0099af20.cpp).
    bool Contains(const CWideString& other) const;
    bool operator<=(const CWideString& other) const;
    bool operator>(const CWideString& other) const;
    bool operator>=(const CWideString& other) const;
    void SaveToFile(CAFile& file);
    bool operator!=(wchar_t character) const;
    unsigned long GetCRC() const;

    // Retail inlines this everywhere; the data is a first/last/end vector.
    __forceinline bool operator==(const wchar_t* other) const
    {
        // The empty case as an if/return pair: VC7.1 then folds it into the
        // caller's branch (CAFile::GetCurrentPath 0x00997F80); `return
        // wcscmp(...) == 0` materialises the bool there instead.
        if (storage_ == 0)
        {
            if (wcscmp(other, L"") == 0)
                return true;
            return false;
        }
        unsigned long length = 0;
        while (other[length] != 0)
            ++length;
        const wchar_t* first = storage_->text;
        const wchar_t* last =
            reinterpret_cast<const wchar_t*>(storage_->unknown04);
        return
            static_cast<unsigned long>(last - first) == length &&
            MemCmp_Unsigned16(first, other, length) == 0;
    }

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    void MakeOwnCopyOfData();
    // Ego_r, retail 0x0099B5E0.
    void MakeOwnCopyOfDataOrEmptyString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);
// lib_wide_string.cpp, retail 0x0099C080 / 0x0099C120.
CWideString FABLE_FASTCALL operator+(wchar_t left, const CWideString& right);
CWideString FABLE_FASTCALL operator+(const CWideString& left, wchar_t right);

extern char g_FableEmptyNarrowString_0129AAF4[1];

// Same declaration as fable_crc.h; CCharString::GetCRC inlines a call to it.
fable_u32 FABLE_FASTCALL
FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount);

class CCharString
{
public:
    unsigned long Sizeof() const;
    CCharString();
    // Retail pushes the -1 default at every literal conversion (0x413c50).
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    // Retail 0x0099FA50 reads a CAFile into a new narrow string.
    CCharString(CAFile* file, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    CCharString& operator=(char character);
    // FableWin ??YCCharString@@QAEABV0@PBD@Z (0x0099F100).
    const CCharString& operator+=(const char* text);
    const CCharString& operator+=(char character);
    // FableWin ??YCCharString@@QAEABV0@ABV0@@Z (0x0099F0A0).
    const CCharString& operator+=(const CCharString& other);
    const CCharString& operator=(const char* text);
    bool operator!=(const char* other) const;
    CWideString ToWideString() const;
    CCharString GetAsUpperCase() const;
    // FableWin ?GetAsLowerCase@CCharString@@QBE?AV1@XZ, retail 0x0099ED10.
    CCharString GetAsLowerCase() const;
    // FableWin ?GetSubString@CCharString@@QBE?AV1@KK@Z, retail 0x0099EC70.
    CCharString GetSubString(unsigned long start, unsigned long end) const;
    // FableWin ?Sprintf@CCharString@@QAAXPBDZZ, retail 0x0099F1F0.
    void Sprintf(const char* format, ...);
    void LoadFromFile(CAFile&, CASuspendableProcess*, long);
    long FindCharPos(char character) const;
    // Retail 0x0099F560 calls the leading and trailing trim methods.
    void RemoveWhiteSpaceBeginEnd();
    void RemoveWhiteSpaceBegin();
    void RemoveWhiteSpaceEnd();
    // Retail 0x0099EF40.
    void SetAsSubString(const char* text, long length);
    void SetChar(long index, char character);
    // Ego_r in-place edits: Truncate 0x0099F2A0, Insert 0x0099F3D0, Erase 0x0099F400.
    void Truncate(unsigned long length);
    void Insert(long pos, char c);
    void Erase(long start, long count);
    // Ego_r ?SaveToFile@CCharString@@QBEXAAVCAFile@@@Z, retail 0x0099E5F0.
    void SaveToFile(CAFile& file) const;

    // Retail both calls this (0x0099E960) and expands it in place; a TU that
    // needs the expansion defines FABLE_CHAR_STRING_EQUALS_FORCEINLINE.
#ifdef FABLE_CHAR_STRING_EQUALS_FORCEINLINE
    __forceinline
#endif
    bool operator==(const char* other) const
    {
        if (storage_ == 0)
        {
            if (strcmp(other, "") == 0)
                return true;
            return false;
        }
        return *storage_ == other;
    }
    // Ego_r inline operator==(char): a one-character string compare; null
    // storage is never equal (operator!=(char) 0x0099E520, CConsole::
    // SetHelpBarString 0x009EB2C0).
    __forceinline bool operator==(char c) const
    {
        if (storage_ != 0)
        {
            char text[2];
            text[0] = c;
            text[1] = 0;
            return strcmp(storage_->text, text) == 0;
        }
        return false;
    }
    // Ego_r ??9CCharString@@QBE_ND@Z, retail 0x0099E520.
    bool operator!=(char c) const;
    // FableWin ??9CCharString@@QBE_NABV0@@Z, retail 0x0099E900: the negated inline ==
    // (CCharString_NotEqual_0099e900.cpp).
    bool operator!=(const CCharString& other) const;
    // FableWin ??8CCharString@@QBE_NABV0@@Z; inline in retail (0x009E50C0):
    // shared data, then null, then the string comparison.
    bool operator==(const CCharString& other) const
    {
        if (storage_ == other.storage_)
            return true;
        if (storage_ == 0 || other.storage_ == 0)
            return false;
        return storage_->Data == other.storage_->Data;
    }

    // Inlined by CBankFileManager::OpenIniFile (0x9a8170), whose callers pass
    // literals through the implicit conversion temporary.
    bool EqualsCaseInsensitive(const CCharString& other) const
    {
        return _stricmp(GetConstCharPtr(), other.GetConstCharPtr()) == 0;
    }

    // Ego_r inline StartsWith (SetGuessMode 0x009EAFF0 calls strncmp over the
    // argument's length).
    __forceinline bool StartsWith(const CCharString& other) const
    {
        return strncmp(GetConstCharPtr(), other.GetConstCharPtr(), other.GetLength()) == 0;
    }

    // Ego_r inline EndsWith (CConsole::ConsoleRunScript 0x009ECB70): false
    // when shorter, else strcmp of the tail.
    __forceinline bool EndsWith(const CCharString& other) const
    {
        long offset = GetLength() - other.GetLength();
        if (offset >= 0)
            return strcmp(GetConstCharPtr() + offset, other.GetConstCharPtr()) == 0;
        return false;
    }

    // Ego_r inline Contains: a substring search (the console list commands
    // 0x009EB590.. call strstr on the lower-cased texts).
    bool Contains(const CCharString& other) const
    {
        return strstr(GetConstCharPtr(), other.GetConstCharPtr()) != 0;
    }

    // The inline form of operator const char*: retail inlines it at some call
    // sites and calls folded out-of-line copies at others.
    const char* GetConstCharPtr() const
    {
        if (storage_ == 0)
            return g_FableEmptyNarrowString_0129AAF4;
        return storage_->text;
    }

    long GetByteLength() const
    {
        return storage_ == 0 ? 0 : storage_->unknown04;
    }

    // FableWin's name (?GetLength@CCharString@@QBEJXZ, retail 0x403A00);
    // CSystemManager::Initialise inlines it.
    long GetLength() const
    {
        return storage_ == 0 ? 0 : storage_->unknown04;
    }

    // FableWin ?IsEmpty@CCharString@@QBE_NXZ; inline in retail
    // (CDefStringTable::AddString 0x009D4F90).
    bool IsEmpty() const
    {
        return GetLength() == 0;
    }

    unsigned long GetCRC() const
    {
        return FableCRC_Calc_004014A0(0, GetConstCharPtr(), GetByteLength());
    }

    // FableWin ?FastCompare@CCharString@@QBE_NABV1@@Z: a cheap strict order
    // (shared data equal, null first, then shorter first, then dword-wise);
    // out of line in retail (the CCompareString copy is 0x009E6530).
    bool FastCompare(const CCharString& other) const;

    // Shared data compares equal by identity; a null string sorts first; then
    // CBasicString<char>::operator< 0x00429950 on the text (map<CCharString,
    // long>::operator[] 0x00519230 calls it, _M_lower_bound 0x0047547D expands it).
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.storage_ == storage_)
            return false;
        if (storage_ == 0)
            return true;
        if (other.storage_ == 0)
            return false;
        return storage_->Data < other.storage_->Data;
    }

protected:
    // FableWin ?MakeOwnCopyOfDataOrEmptyString@CCharString@@IAEXXZ, retail 0x0099EB40.
    void MakeOwnCopyOfDataOrEmptyString();

private:
    void UnassignString();
    // FableWin ?MakeOwnCopyOfData@CCharString@@AAEXXZ, retail 0x0099EAF0:
    // copy shared data before a write.
    void MakeOwnCopyOfData();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// FableWin ??H@YI?AVCCharString@@PBDABV0@@Z, retail 0x0099F690 (the result
// pointer in ECX, the literal in EDX, the string on the stack).
CCharString FABLE_FASTCALL operator+(
    const char* left,
    const CCharString& right);

// FableWin ??H@YI?AVCCharString@@ABV0@D@Z, retail 0x0099F7A0.
CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    char right);

// FableWin ??H@YI?AVCCharString@@ABV0@0@Z, retail 0x0099F570.
CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const CCharString& right);

// Ego_r ?GFIsCharStringNumber@@YI_NABVCCharString@@@Z, retail 0x0099E620.
bool FABLE_FASTCALL GFIsCharStringNumber(const CCharString& cstr);
// Ego_r ?GFCharStringToInt@@YIJABVCCharString@@@Z, retail 0x0099E7F0.
// Ego_r ?GFHexCharStringToInt@@YIKABVCCharString@@@Z, retail 0x0099E750.
unsigned long FABLE_FASTCALL GFHexCharStringToInt(const CCharString& text);
long FABLE_FASTCALL GFCharStringToInt(const CCharString& cstr);
// Ego_r ?GFCharStringToUHUGE@@YI_KABVCCharString@@@Z, retail 0x0099E870.
unsigned __int64 FABLE_FASTCALL GFCharStringToUHUGE(const CCharString& cstr);
// FableWin ??H@YI?AVCCharString@@PBDABV0@@Z, retail 0x0099F690.
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right);
// FableWin ?GFIntToCharString@@YI?AVCCharString@@J@Z, retail 0x0099F830.
CCharString FABLE_FASTCALL GFIntToCharString(long value);
// Ego_r ?GFBoolToCharString@@YI?AVCCharString@@_N@Z, retail 0x0099F9B0 ("TRUE"/"FALSE").
CCharString FABLE_FASTCALL GFBoolToCharString(bool val);
// Ego_r ?GFByteSizeToCharString@@YI?AVCCharString@@KJ@Z, retail 0x0099FA90.
CCharString FABLE_FASTCALL GFByteSizeToCharString(unsigned long size, long precision);
// Ego_r ?GFUIntToCharString@@YI?AVCCharString@@K@Z, retail 0x0099F8B0 ("%u").
CCharString FABLE_FASTCALL GFUIntToCharString(unsigned long n);
// Ego_r ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z, retail 0x0099F930 ("%I64u").
CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64 n);
// Retail 0x009A08B0; donor global formatter.
CCharString FABLE_FASTCALL GFFloatToCharString(double value, long precision);
// FableWin ?GFGetLastWindowsError@@YI?AVCCharString@@XZ, retail 0x00A5ED60.
CCharString FABLE_FASTCALL GFGetLastWindowsError();

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);
