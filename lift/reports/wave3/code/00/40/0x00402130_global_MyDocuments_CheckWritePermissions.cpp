#include <cstddef>
#include <cstdint>
#include <cstdio>

struct CCharString
{
    std::uint32_t opaque;
};
static_assert(sizeof(CCharString) == 4);

using DWORD = unsigned long;
using BOOL = int;
using HANDLE = void*;
using LPCWSTR = wchar_t const*;

struct _SECURITY_ATTRIBUTES;
using LPSECURITY_ATTRIBUTES = _SECURITY_ATTRIBUTES*;

#pragma pack(push, 1)
struct WideSpan
{
    wchar_t* begin; // 0x00
    wchar_t* end;   // 0x04
};
static_assert(offsetof(WideSpan, begin) == 0x00);
static_assert(offsetof(WideSpan, end) == 0x04);
static_assert(sizeof(WideSpan) == 0x08);

struct CCharStringWideOverlay
{
    WideSpan* span; // 0x00
};
static_assert(offsetof(CCharStringWideOverlay, span) == 0x00);
static_assert(sizeof(CCharStringWideOverlay) == 0x04);

struct ByteStringRep
{
    char* data;            // 0x00
    std::uint32_t length;  // 0x04
};
static_assert(offsetof(ByteStringRep, data) == 0x00);
static_assert(offsetof(ByteStringRep, length) == 0x04);
static_assert(sizeof(ByteStringRep) == 0x08);

struct CCharStringByteOverlay
{
    ByteStringRep* rep; // 0x00
};
static_assert(offsetof(CCharStringByteOverlay, rep) == 0x00);
static_assert(sizeof(CCharStringByteOverlay) == 0x04);
#pragma pack(pop)

extern wchar_t const DAT_0122d70c[];
extern char const DAT_0122d70e[];
extern wchar_t const DAT_0122d750[];
extern wchar_t const DAT_0122d818[];
extern char DAT_0129aaf4;

extern "C" void __thiscall CCharString_ctor(CCharString* self);
extern "C" void __thiscall CCharString_dtor(CCharString* self);
extern "C" CCharString* __thiscall CCharString__SetSubstring(
    CCharString const* self,
    CCharString* destination,
    int start,
    int count);
extern "C" CCharString* __thiscall CCharString_operator_assign_text(
    CCharString* self,
    char const* text);
extern "C" void __thiscall CCharString__AssignFromValue(
    CCharString* destination,
    CCharString const* source,
    wchar_t const* value);
extern "C" LPCWSTR __thiscall CCharString_operator_char_const_(CCharString const* self);
extern "C" char __thiscall CCharString__NotEqual(CCharString const* self, char const* text);

extern "C" int __cdecl wcscmp(wchar_t const* lhs, wchar_t const* rhs);
extern "C" int __cdecl MemCmp_Unsigned16(wchar_t const* lhs, wchar_t const* rhs, int count);
extern "C" HANDLE __stdcall CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);
extern "C" BOOL __stdcall CloseHandle(HANDLE hObject);
extern "C" std::FILE* __cdecl _wfopen(wchar_t const* filename, wchar_t const* mode);
extern "C" int __cdecl fclose(std::FILE* stream);
extern "C" BOOL __stdcall DeleteFileW(LPCWSTR lpFileName);
extern "C" std::uint32_t __cdecl CCRC__Calc(std::uint32_t seed, void const* data, std::uint32_t length);

extern "C" std::uint32_t __thiscall MyDocuments_CheckWritePermissions(CCharString const* myDocumentsPath)
{
    CCharString errorText;
    CCharString tempPathOrPrefix;

    CCharString_ctor(&errorText);

    CCharString* const substringObject =
        CCharString__SetSubstring(myDocumentsPath, &tempPathOrPrefix, 0, 2);
    WideSpan* const substringSpan =
        reinterpret_cast<CCharStringWideOverlay*>(substringObject)->span;

    char isRedirected;
    if (substringSpan == nullptr)
    {
        const int compareResult = wcscmp(DAT_0122d818, DAT_0122d70c);
        isRedirected = static_cast<char>(1 - (compareResult != 0));
    }
    else
    {
        int prefixLength = 0;
        do
        {
            ++prefixLength;
        } while (DAT_0122d818[prefixLength] != L'\0');

        if ((substringSpan->end - substringSpan->begin) == prefixLength)
        {
            const int compareResult =
                MemCmp_Unsigned16(substringSpan->begin, DAT_0122d818, prefixLength);
            if (compareResult == 0)
            {
                isRedirected = 1;
                goto after_prefix_compare;
            }
        }

        isRedirected = 0;
    }

after_prefix_compare:
    CCharString_dtor(&tempPathOrPrefix);

    if (isRedirected != 0)
    {
        CCharString_operator_assign_text(
            &errorText,
            "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED");
        goto done_path_check;
    }

    CCharString__AssignFromValue(&tempPathOrPrefix, myDocumentsPath, DAT_0122d750);

    {
        HANDLE fileHandle = CreateFileW(
            CCharString_operator_char_const_(&tempPathOrPrefix),
            0x10000000,
            3,
            nullptr,
            2,
            0,
            nullptr);

        if (fileHandle == reinterpret_cast<HANDLE>(-1))
        {
            CCharString_operator_assign_text(
                &errorText,
                "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED");
        }
        else
        {
            CloseHandle(fileHandle);

            std::FILE* const file =
                _wfopen(CCharString_operator_char_const_(&tempPathOrPrefix), L"w+");
            if (file == nullptr)
            {
                CCharString_operator_assign_text(
                    &errorText,
                    "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED");
            }
            else
            {
                fclose(file);

                if (DeleteFileW(CCharString_operator_char_const_(&tempPathOrPrefix)) == 0)
                {
                    CCharString_operator_assign_text(
                        &errorText,
                        "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED");
                    goto restricted_done;
                }
            }
        }
    }

restricted_done:
    CCharString_dtor(&tempPathOrPrefix);

done_path_check:
    if (CCharString__NotEqual(&errorText, DAT_0122d70e) != 0)
    {
        ByteStringRep* const rep = reinterpret_cast<CCharStringByteOverlay*>(&errorText)->rep;

        std::uint32_t length;
        void const* data;
        if (rep == nullptr)
        {
            length = 0;
            data = &DAT_0129aaf4;
        }
        else
        {
            length = rep->length;
            data = rep->data;
        }

        const std::uint32_t crc = CCRC__Calc(0, data, length);
        CCharString_dtor(&errorText);
        return crc;
    }

    CCharString_dtor(&errorText);
    return 0;
}