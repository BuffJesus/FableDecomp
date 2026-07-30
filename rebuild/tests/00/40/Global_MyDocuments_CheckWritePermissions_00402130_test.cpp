#include <stdio.h>
#include <string.h>

#include "fable_write_permissions.h"

extern "C" fable_u8 g_FableWriteEmptyWide_0122D70C = 0;
extern "C" fable_u8 g_FableWriteEmptyNarrow_0122D70E = 0;
extern "C" fable_u8 g_FableWriteRestrictedKey_0122D710 = 0;
extern "C" fable_u8 g_FableWriteOpenMode_0122D744 = 0;
extern "C" fable_u8 g_FableWriteProbeSuffix_0122D750 = 0;
extern "C" fable_u8 g_FableWriteRedirectedKey_0122D7E4 = 0;
extern "C" wchar_t g_FableWriteRedirectedPrefix_0122D818[] = L"\\\\";
extern "C" fable_u8 g_FableWriteEmptyCharData_0129AAF4 = 0;

namespace
{
    struct WideSpan
    {
        const wchar_t* begin;
        const wchar_t* end;
    };

    struct NarrowData
    {
        const char* text;
        fable_u32 length;
    };

    enum Failure
    {
        kSuccess,
        kCreateFailure,
        kOpenFailure,
        kDeleteFailure
    };

    const wchar_t g_localPrefix[] = L"C:";
    const wchar_t g_redirectPrefix[] = L"\\\\";
    const wchar_t g_probePath[] = L"C:\\probe.tmp";
    const char g_restrictedKey[] =
        "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
    const char g_redirectedKey[] =
        "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    WideSpan g_span;
    NarrowData g_errorData;
    bool g_redirected;
    Failure g_failure;
    bool g_argumentsValid;
    unsigned int g_createCalls;
    unsigned int g_closeHandleCalls;
    unsigned int g_openCalls;
    unsigned int g_closeFileCalls;
    unsigned int g_deleteCalls;
    unsigned int g_wideDestroyCalls;
    unsigned int g_charDestroyCalls;
    const char* g_assignedKey;
    fable_u32 g_crc;

    void Reset(bool redirected, Failure failure)
    {
        g_redirected = redirected;
        g_failure = failure;
        g_argumentsValid = true;
        g_createCalls = 0;
        g_closeHandleCalls = 0;
        g_openCalls = 0;
        g_closeFileCalls = 0;
        g_deleteCalls = 0;
        g_wideDestroyCalls = 0;
        g_charDestroyCalls = 0;
        g_assignedKey = 0;
        g_crc = 0;

        const wchar_t* prefix =
            redirected ? g_redirectPrefix : g_localPrefix;
        g_span.begin = prefix;
        g_span.end = prefix + 2;
        g_errorData.text = 0;
        g_errorData.length = 0;
    }

    bool Run(bool redirected, Failure failure, bool expectError)
    {
        Reset(redirected, failure);
        fable_u8 pathBytes[sizeof(CWideString)] = {0};
        const CWideString& path =
            *reinterpret_cast<const CWideString*>(pathBytes);
        const fable_u32 result =
            FableCheckMyDocumentsWritePermissions_00402130(path);
        return
            g_argumentsValid &&
            (expectError ? result == g_crc && result != 0 : result == 0) &&
            g_charDestroyCalls == 1;
    }
}

extern "C" void FABLE_FASTCALL
FableWriteCharConstruct_00402130(void* string, void*)
{
    *reinterpret_cast<void**>(string) = 0;
}

extern "C" void* FABLE_FASTCALL
FableWriteWideSubstring_00402130(
    const void*,
    void*,
    void* result,
    fable_i32 start,
    fable_i32 count)
{
    g_argumentsValid =
        g_argumentsValid &&
        start == 0 &&
        count == 2;
    *reinterpret_cast<WideSpan**>(result) = &g_span;
    return result;
}

extern "C" fable_i32 FABLE_CDECL
FableWriteWideCompare_00402130(
    const wchar_t* left,
    const wchar_t* right)
{
    return wcscmp(left, right);
}

extern "C" fable_i32 FABLE_CDECL
FableWriteWideMemoryCompare_00402130(
    const wchar_t* left,
    const wchar_t* right,
    fable_i32 count)
{
    return memcmp(left, right, count * sizeof(wchar_t));
}

extern "C" void FABLE_FASTCALL
FableWriteWideDestroy_00402130(void* string, void*)
{
    ++g_wideDestroyCalls;
    *reinterpret_cast<void**>(string) = 0;
}

extern "C" void* FABLE_FASTCALL
FableWriteCharAssignLiteral_00402130(
    void* string,
    void*,
    const char* literal)
{
    const char* restricted =
        reinterpret_cast<const char*>(&g_FableWriteRestrictedKey_0122D710);
    const char* redirected =
        reinterpret_cast<const char*>(&g_FableWriteRedirectedKey_0122D7E4);
    g_assignedKey =
        literal == redirected ? g_redirectedKey :
        literal == restricted ? g_restrictedKey :
        0;
    g_argumentsValid =
        g_argumentsValid &&
        g_assignedKey != 0;
    g_errorData.text = g_assignedKey;
    g_errorData.length =
        static_cast<fable_u32>(strlen(g_assignedKey));
    *reinterpret_cast<NarrowData**>(string) = &g_errorData;
    return string;
}

extern "C" void FABLE_FASTCALL
FableWriteWideAppendLiteral_00402130(
    void* result,
    const void*,
    const wchar_t* right)
{
    g_argumentsValid =
        g_argumentsValid &&
        right ==
            reinterpret_cast<const wchar_t*>(
                &g_FableWriteProbeSuffix_0122D750);
    *reinterpret_cast<const wchar_t**>(result) = g_probePath;
}

extern "C" const wchar_t* FABLE_FASTCALL
FableWriteWideConversion_00402130(const void*, void*)
{
    return g_probePath;
}

extern "C" void* FABLE_CDECL
FableWriteWideFileOpen_00402130(
    const wchar_t* filename,
    const wchar_t*)
{
    ++g_openCalls;
    g_argumentsValid =
        g_argumentsValid &&
        filename == g_probePath;
    return g_failure == kOpenFailure ? 0 : &g_span;
}

extern "C" fable_i32 FABLE_CDECL
FableWriteFileClose_00402130(void* stream)
{
    ++g_closeFileCalls;
    g_argumentsValid =
        g_argumentsValid &&
        stream == &g_span;
    return 0;
}

extern "C" bool FABLE_FASTCALL
FableWriteCharNotEqual_00402130(
    const void* string,
    void*,
    const char*)
{
    return *reinterpret_cast<NarrowData* const*>(string) != 0;
}

extern "C" fable_u32 FABLE_FASTCALL
FableWriteCrc_00402130(
    fable_u32 seed,
    const void* data,
    fable_u32 length)
{
    g_argumentsValid =
        g_argumentsValid &&
        seed == 0 &&
        data == g_assignedKey &&
        length == strlen(g_assignedKey);
    g_crc =
        g_assignedKey == g_redirectedKey ?
            0xD1EC7EDU :
            0x5E571C7U;
    return g_crc;
}

extern "C" void FABLE_FASTCALL
FableWriteCharDestroy_00402130(void* string, void*)
{
    ++g_charDestroyCalls;
    *reinterpret_cast<void**>(string) = 0;
}

extern "C" void* __stdcall
FableWriteCreateFileStub_00402130(
    const wchar_t* filename,
    unsigned long desiredAccess,
    unsigned long shareMode,
    void*,
    unsigned long creationDisposition,
    unsigned long flags,
    void*)
{
    ++g_createCalls;
    g_argumentsValid =
        g_argumentsValid &&
        filename == g_probePath &&
        desiredAccess == 0x10000000 &&
        shareMode == 3 &&
        creationDisposition == 2 &&
        flags == 0;
    return
        g_failure == kCreateFailure ?
            reinterpret_cast<void*>(-1) :
            &g_errorData;
}

extern "C" int __stdcall
FableWriteCloseHandleStub_00402130(void* handle)
{
    ++g_closeHandleCalls;
    g_argumentsValid =
        g_argumentsValid &&
        handle == &g_errorData;
    return 1;
}

extern "C" int __stdcall
FableWriteDeleteFileStub_00402130(const wchar_t* filename)
{
    ++g_deleteCalls;
    g_argumentsValid =
        g_argumentsValid &&
        filename == g_probePath;
    return g_failure == kDeleteFailure ? 0 : 1;
}

extern "C" void* g_FableCreateFileW_0143FE2C =
    FableWriteCreateFileStub_00402130;
extern "C" void* g_FableCloseHandle_0143FE30 =
    FableWriteCloseHandleStub_00402130;
extern "C" void* g_FableDeleteFileW_0143FE34 =
    FableWriteDeleteFileStub_00402130;

int main()
{
    const bool redirected =
        Run(true, kSuccess, true) &&
        g_assignedKey == g_redirectedKey &&
        g_createCalls == 0 &&
        g_wideDestroyCalls == 1;
    const bool success =
        Run(false, kSuccess, false) &&
        g_assignedKey == 0 &&
        g_createCalls == 1 &&
        g_closeHandleCalls == 1 &&
        g_openCalls == 1 &&
        g_closeFileCalls == 1 &&
        g_deleteCalls == 1 &&
        g_wideDestroyCalls == 2;
    const bool createFailure =
        Run(false, kCreateFailure, true) &&
        g_assignedKey == g_restrictedKey &&
        g_createCalls == 1 &&
        g_closeHandleCalls == 0 &&
        g_openCalls == 0 &&
        g_deleteCalls == 0;
    const bool openFailure =
        Run(false, kOpenFailure, true) &&
        g_closeHandleCalls == 1 &&
        g_openCalls == 1 &&
        g_closeFileCalls == 0 &&
        g_deleteCalls == 0;
    const bool deleteFailure =
        Run(false, kDeleteFailure, true) &&
        g_closeFileCalls == 1 &&
        g_deleteCalls == 1;

    if (!(
        redirected &&
        success &&
        createFailure &&
        openFailure &&
        deleteFailure))
    {
        printf(
            "FABLETLC_MY_DOCUMENTS_WRITE_PERMISSIONS FAIL "
            "redirected=%d success=%d create=%d open=%d delete=%d\n",
            redirected,
            success,
            createFailure,
            openFailure,
            deleteFailure);
        return 1;
    }

    printf("FABLETLC_MY_DOCUMENTS_WRITE_PERMISSIONS PASS\n");
    return 0;
}
