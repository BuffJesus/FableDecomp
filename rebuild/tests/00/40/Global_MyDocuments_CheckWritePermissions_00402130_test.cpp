#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// kernel32 (the VC7.1 toolset here has no Platform SDK headers).
extern "C"
{
    __declspec(dllimport) void* __stdcall CreateFileW(
        const wchar_t*, unsigned long, unsigned long, void*,
        unsigned long, unsigned long, void*);
    __declspec(dllimport) int __stdcall CloseHandle(void*);
    __declspec(dllimport) int __stdcall DeleteFileW(const wchar_t*);
    __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(
        const wchar_t*);
    __declspec(dllimport) unsigned long __stdcall GetTempPathW(
        unsigned long, wchar_t*);
    __declspec(dllimport) int __stdcall CreateDirectoryW(
        const wchar_t*, void*);
    __declspec(dllimport) int __stdcall RemoveDirectoryW(const wchar_t*);
}

#define MAX_PATH 260
#define INVALID_HANDLE_VALUE (reinterpret_cast<void*>(-1))
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFFu
#define GENERIC_ALL 0x10000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 1u
#define FILE_SHARE_WRITE 2u
#define CREATE_ALWAYS 2u
#define OPEN_EXISTING 3u

#include "fable_write_permissions.h"

// Recorders for the real callees. The kernel32 import slots forward to the
// real API, so the probe runs against a real temporary folder. A CWideString's
// storage is a CWideStringData whose first/last words bound its text.
char g_FableEmptyNarrowString_0129AAF4[1] = {0};

namespace
{
    const char kRestricted[] = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
    const char kRedirected[] = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    const wchar_t kProbeSuffix[] =
        L"This directory has restricted permissions, and Fable cannot run properly";

    struct WideRecord
    {
        // Raw storage: CWideStringData has no default ctor.
        unsigned long dataRaw_[4];
        CWideStringData& Data() { return *reinterpret_cast<CWideStringData*>(dataRaw_); }
        wchar_t text[MAX_PATH * 2];
    };

    WideRecord g_wide[8];
    unsigned int g_wideUsed;
    CCharStringData g_errorData;

    enum Override
    {
        kReal,
        kCreateSucceedsOnDevice,
        kDeleteFails
    };

    Override g_override;
    unsigned int g_createCalls;
    unsigned int g_openedFiles;
    unsigned int g_deleteCalls;
    unsigned int g_subStringCalls;
    unsigned int g_wideDestructs;
    unsigned int g_charDestructs;
    const char* g_crcText;
    wchar_t g_probePath[MAX_PATH * 2];

    CWideStringData*& Storage(CWideString& value)
    {
        return *reinterpret_cast<CWideStringData**>(&value);
    }

    const CWideStringData* Storage(const CWideString& value)
    {
        return *reinterpret_cast<CWideStringData* const*>(&value);
    }

    CWideStringData* MakeWide(const wchar_t* text, size_t length)
    {
        WideRecord& record = g_wide[g_wideUsed++];
        wcsncpy(record.text, text, length);
        record.text[length] = 0;
        record.Data().text = record.text;
        record.Data().unknown04 =
            reinterpret_cast<fable_u32>(record.text + length);
        record.Data().unknown08 = record.Data().unknown04;
        record.Data().owners = 1;
        return &record.Data();
    }

    void* __stdcall CreateFileRecorder(
        const wchar_t* fileName,
        unsigned long desiredAccess,
        unsigned long shareMode,
        void* securityAttributes,
        unsigned long creationDisposition,
        unsigned long flagsAndAttributes,
        void* templateFile)
    {
        ++g_createCalls;
        wcscpy(g_probePath, fileName);
        if (desiredAccess != GENERIC_ALL ||
            shareMode != (FILE_SHARE_READ | FILE_SHARE_WRITE) ||
            creationDisposition != CREATE_ALWAYS ||
            flagsAndAttributes != 0)
            return INVALID_HANDLE_VALUE;
        if (g_override == kCreateSucceedsOnDevice)
            return CreateFileW(L"NUL", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
        return CreateFileW(
            fileName,
            desiredAccess,
            shareMode,
            securityAttributes,
            creationDisposition,
            flagsAndAttributes,
            templateFile);
    }

    int __stdcall CloseHandleRecorder(void* handle)
    {
        return CloseHandle(handle);
    }

    int __stdcall DeleteFileRecorder(const wchar_t* fileName)
    {
        ++g_deleteCalls;
        if (g_override == kDeleteFails)
        {
            DeleteFileW(fileName);
            return 0;
        }
        return DeleteFileW(fileName);
    }

    void Reset(Override override)
    {
        g_wideUsed = 0;
        g_override = override;
        g_createCalls = 0;
        g_deleteCalls = 0;
        g_subStringCalls = 0;
        g_wideDestructs = 0;
        g_charDestructs = 0;
        g_crcText = 0;
        g_probePath[0] = 0;
    }

    fable_u32 Check(const wchar_t* path)
    {
        CWideString input;
        Storage(input) = MakeWide(path, wcslen(path));
        const fable_u32 result =
            FableCheckMyDocumentsWritePermissions_00402130(input);
        Storage(input) = 0;
        return result;
    }

    bool FileExists(const wchar_t* path)
    {
        return GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
    }
}

typedef void* (__stdcall* CreateFileSlot)(
    const wchar_t*, unsigned long, unsigned long, void*,
    unsigned long, unsigned long, void*);
typedef int (__stdcall* CloseHandleSlot)(void*);
typedef int (__stdcall* DeleteFileSlot)(const wchar_t*);

extern "C" CreateFileSlot g_FableCreateFileW_0143FE2C = &CreateFileRecorder;
extern "C" CloseHandleSlot g_FableCloseHandle_0143FE30 = &CloseHandleRecorder;
extern "C" DeleteFileSlot g_FableDeleteFileW_0143FE34 = &DeleteFileRecorder;

CWideString::CWideString()
    : storage_(0)
{
}

CWideString::~CWideString()
{
    ++g_wideDestructs;
}

CWideString::operator const wchar_t*() const
{
    return storage_ != 0 ? storage_->text : L"";
}

CWideString::CWideString(const wchar_t* text)
    : storage_(MakeWide(text, wcslen(text)))
{
}

CWideString CWideString::GetSubString(
    unsigned long start,
    unsigned long count) const
{
    ++g_subStringCalls;
    wchar_t buffer[MAX_PATH * 2] = {0};
    const wchar_t* text = storage_ != 0 ? storage_->text : L"";
    const size_t length = wcslen(text);
    if (start < length)
    {
        const size_t available = length - start;
        wcsncpy(buffer, text + start, count < available ? count : available);
    }
    return CWideString(buffer);
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    wchar_t buffer[MAX_PATH * 2];
    wcscpy(buffer, static_cast<const wchar_t*>(left));
    wcscat(buffer, right);
    return CWideString(buffer);
}

int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs,
    const unsigned short* rhs,
    unsigned long count)
{
    for (unsigned long index = 0; index < count; ++index)
    {
        if (lhs[index] != rhs[index])
            return lhs[index] < rhs[index] ? -1 : 1;
    }
    return 0;
}

CCharString::CCharString()
    : storage_(0)
{
}

CCharString::~CCharString()
{
    ++g_charDestructs;
}

const CCharString& CCharString::operator=(const char* text)
{
    g_errorData.text = const_cast<char*>(text);
    g_errorData.unknown04 = static_cast<fable_u32>(strlen(text));
    storage_ = &g_errorData;
    return *this;
}

bool CCharString::operator!=(const char* other) const
{
    const char* text = storage_ != 0 ? storage_->text : "";
    return strcmp(text, other) != 0;
}

fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount)
{
    const char* text = static_cast<const char*>(data);
    g_crcText =
        seed == 0 && byteCount == strlen(text) ? text : "bad-crc-input";
    return 0xC0DE0000u + byteCount;
}

int main()
{
    wchar_t folder[MAX_PATH];
    wchar_t probe[MAX_PATH * 2];
    const char* failure = 0;

    GetTempPathW(MAX_PATH, folder);
    wcscat(folder, L"fabletlc_write_probe\\");
    CreateDirectoryW(folder, 0);
    wcscpy(probe, folder);
    wcscat(probe, kProbeSuffix);
    DeleteFileW(probe);

    // A writable local folder: the probe file is created, reopened, deleted.
    Reset(kReal);
    fable_u32 result = Check(folder);
    if (result != 0 || g_createCalls != 1 || g_deleteCalls != 1 ||
        wcscmp(g_probePath, probe) != 0 || FileExists(probe) ||
        g_subStringCalls != 1 || g_charDestructs != 1 ||
        g_wideDestructs != 3)
        failure = "writable";

    // A redirected (UNC) folder is rejected before touching the disk.
    Reset(kReal);
    result = Check(L"\\\\server\\share\\My Documents\\");
    if (!failure &&
        (result != 0xC0DE0000u + sizeof(kRedirected) - 1 ||
         g_crcText == 0 || strcmp(g_crcText, kRedirected) != 0 ||
         g_createCalls != 0 || g_charDestructs != 1 || g_wideDestructs != 2))
        failure = "redirected";

    // A missing folder: CreateFileW fails.
    Reset(kReal);
    result = Check(L"Z:\\fabletlc\\definitely\\missing\\");
    if (!failure &&
        (result != 0xC0DE0000u + sizeof(kRestricted) - 1 ||
         strcmp(g_crcText, kRestricted) != 0 ||
         g_createCalls != 1 || g_deleteCalls != 0 || g_wideDestructs != 3))
        failure = "create";

    // CreateFileW succeeds but the CRT cannot open the probe path.
    Reset(kCreateSucceedsOnDevice);
    result = Check(L"Z:\\fabletlc\\definitely\\missing\\");
    if (!failure &&
        (result != 0xC0DE0000u + sizeof(kRestricted) - 1 ||
         strcmp(g_crcText, kRestricted) != 0 ||
         g_createCalls != 1 || g_deleteCalls != 0))
        failure = "open";

    // The probe cannot be deleted.
    Reset(kDeleteFails);
    result = Check(folder);
    if (!failure &&
        (result != 0xC0DE0000u + sizeof(kRestricted) - 1 ||
         strcmp(g_crcText, kRestricted) != 0 ||
         g_deleteCalls != 1))
        failure = "delete";

    DeleteFileW(probe);
    RemoveDirectoryW(folder);

    if (failure != 0)
    {
        printf("FABLETLC_MY_DOCUMENTS_WRITE_PERMISSIONS FAIL %s\n", failure);
        return 1;
    }

    printf("FABLETLC_MY_DOCUMENTS_WRITE_PERMISSIONS PASS\n");
    return 0;
}
