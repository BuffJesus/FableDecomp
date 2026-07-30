// Exact Phase 6 ownership routes used by complete and visual boot.
#include "fable_definition_table.h"
#include "fable_gfmain.h"
#include "fable_write_permissions.h"

#include <string.h>
#include <wchar.h>

extern "C" fable_u8 g_FableDiskFileVTable_0122D06C = 0;
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
    CDefStringTable g_DefinitionTable;
    GFMainPhase6State* g_ActivePhase6State;
    void* g_ExactFile;
    void* g_ExactStream;

    struct FableWideSpan
    {
        const wchar_t* begin;
        const wchar_t* end;
    };

    struct FableNarrowData
    {
        const char* text;
        fable_u32 length;
    };

    const CWideString* g_ExactPermissionPath;
    FableWideSpan g_ExactPermissionPrefix;
    FableNarrowData g_ExactPermissionError;
    wchar_t g_ExactPermissionProbePath[160];

    void InvalidateArguments()
    {
        if (g_ActivePhase6State != 0)
            g_ActivePhase6State->exactDefinitionTableArgumentsValid = false;
    }
}

extern "C" void FABLE_FASTCALL
FableWriteCharConstruct_00402130(void* string, void*)
{
    *static_cast<void**>(string) = 0;
}

extern "C" void* FABLE_FASTCALL
FableWriteWideSubstring_00402130(
    const void* source,
    void*,
    void* result,
    fable_i32 start,
    fable_i32 count)
{
    if (
        source != g_ExactPermissionPath ||
        start != 0 ||
        count != 2)
    {
        InvalidateArguments();
    }
    *static_cast<FableWideSpan**>(result) = &g_ExactPermissionPrefix;
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
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionWideDestroyCalls;
    *static_cast<void**>(string) = 0;
}

extern "C" void* FABLE_FASTCALL
FableWriteCharAssignLiteral_00402130(
    void* string,
    void*,
    const char* literal)
{
    static const char kRestricted[] =
        "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
    static const char kRedirected[] =
        "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    const char* restricted =
        reinterpret_cast<const char*>(&g_FableWriteRestrictedKey_0122D710);
    const char* redirected =
        reinterpret_cast<const char*>(&g_FableWriteRedirectedKey_0122D7E4);
    g_ExactPermissionError.text =
        literal == redirected ? kRedirected :
        literal == restricted ? kRestricted :
        0;
    if (g_ExactPermissionError.text == 0)
        InvalidateArguments();
    g_ExactPermissionError.length =
        g_ExactPermissionError.text != 0
            ? static_cast<fable_u32>(
                strlen(g_ExactPermissionError.text))
            : 0;
    *static_cast<FableNarrowData**>(string) =
        &g_ExactPermissionError;
    return string;
}

extern "C" void FABLE_FASTCALL
FableWriteWideAppendLiteral_00402130(
    void* result,
    const void* left,
    const wchar_t* right)
{
    if (
        left != g_ExactPermissionPath ||
        right != reinterpret_cast<const wchar_t*>(
            &g_FableWriteProbeSuffix_0122D750))
    {
        InvalidateArguments();
    }
    *static_cast<const wchar_t**>(result) =
        g_ExactPermissionProbePath;
}

extern "C" const wchar_t* FABLE_FASTCALL
FableWriteWideConversion_00402130(const void*, void*)
{
    return g_ExactPermissionProbePath;
}

extern "C" void* FABLE_CDECL
FableWriteWideFileOpen_00402130(
    const wchar_t* filename,
    const wchar_t* mode)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionOpenCalls;
    if (
        filename != g_ExactPermissionProbePath ||
        mode != reinterpret_cast<const wchar_t*>(
            &g_FableWriteOpenMode_0122D744))
    {
        InvalidateArguments();
    }
    return &g_ExactPermissionPrefix;
}

extern "C" fable_i32 FABLE_CDECL
FableWriteFileClose_00402130(void* stream)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionCloseFileCalls;
    if (stream != &g_ExactPermissionPrefix)
        InvalidateArguments();
    return 0;
}

extern "C" bool FABLE_FASTCALL
FableWriteCharNotEqual_00402130(
    const void* string,
    void*,
    const char*)
{
    return *static_cast<FableNarrowData* const*>(string) != 0;
}

extern "C" fable_u32 FABLE_FASTCALL
FableWriteCrc_00402130(
    fable_u32,
    const void*,
    fable_u32)
{
    InvalidateArguments();
    return 0;
}

extern "C" void FABLE_FASTCALL
FableWriteCharDestroy_00402130(void* string, void*)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionCharDestroyCalls;
    *static_cast<void**>(string) = 0;
}

extern "C" void* FABLE_STDCALL
FableWriteCreateFileStub_00402130(
    const wchar_t* filename,
    fable_u32 desiredAccess,
    fable_u32 shareMode,
    void*,
    fable_u32 creationDisposition,
    fable_u32 flags,
    void*)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionCreateCalls;
    if (
        filename != g_ExactPermissionProbePath ||
        desiredAccess != 0x10000000 ||
        shareMode != 3 ||
        creationDisposition != 2 ||
        flags != 0)
    {
        InvalidateArguments();
    }
    return &g_ExactPermissionError;
}

extern "C" int FABLE_STDCALL
FableWriteCloseHandleStub_00402130(void* handle)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionCloseHandleCalls;
    if (handle != &g_ExactPermissionError)
        InvalidateArguments();
    return 1;
}

extern "C" int FABLE_STDCALL
FableWriteDeleteFileStub_00402130(const wchar_t* filename)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionDeleteCalls;
    if (filename != g_ExactPermissionProbePath)
        InvalidateArguments();
    return 1;
}

extern "C" void* g_FableCreateFileW_0143FE2C =
    reinterpret_cast<void*>(&FableWriteCreateFileStub_00402130);
extern "C" void* g_FableCloseHandle_0143FE30 =
    reinterpret_cast<void*>(&FableWriteCloseHandleStub_00402130);
extern "C" void* g_FableDeleteFileW_0143FE34 =
    reinterpret_cast<void*>(&FableWriteDeleteFileStub_00402130);

extern "C" bool FABLE_FASTCALL
FableDefTablePathExists_009D4C30(void* pathname, void*)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTablePathExistsCalls;
        if (
            pathname !=
            reinterpret_cast<fable_u8*>(&g_DefinitionTable) + 0x30)
        {
            InvalidateArguments();
        }
    }
    return true;
}

extern "C" void FABLE_FASTCALL
FableDefTableFileBaseConstruct_009D4C30(void* file, void*)
{
    g_ExactFile = file;
}

extern "C" void FABLE_FASTCALL
FableDefTableCharStringConstruct_009D4C30(void*, void*)
{
}

extern "C" void FABLE_FASTCALL
FableDefTableDiskFileOpen_009D4C30(
    void* file,
    void*,
    const void* pathname,
    fable_u32 access,
    fable_u32 flags)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableOpenCalls;
        if (
            file != g_ExactFile ||
            pathname !=
                reinterpret_cast<fable_u8*>(&g_DefinitionTable) + 0x30 ||
            access != 1 ||
            flags != 4)
        {
            InvalidateArguments();
        }
    }
}

extern "C" void FABLE_FASTCALL
FableDefTableInputStreamConstruct_009D4C30(
    void* stream,
    void*,
    void* file,
    fable_u32 bufferSize)
{
    g_ExactStream = stream;
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableStreamConstructCalls;
        if (file != g_ExactFile || bufferSize != 0x4000)
            InvalidateArguments();
    }
}

extern "C" void FABLE_FASTCALL
FableDefTableRead_009D4C30(void* table, void*, void* stream)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableReadCalls;
        if (table != &g_DefinitionTable || stream != g_ExactStream)
            InvalidateArguments();
    }
}

extern "C" void FABLE_FASTCALL
FableDefTableInputStreamDestroy_009D4C30(void* stream, void*)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableStreamDestroyCalls;
        g_ActivePhase6State->exactDefinitionTableLoadedBeforeCleanup =
            g_DefinitionTable.loaded_;
        if (stream != g_ExactStream)
            InvalidateArguments();
    }
}

extern "C" void FABLE_FASTCALL
FableDefTableDiskFileClose_009D4C30(void* file, void*)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableCloseCalls;
        if (file != g_ExactFile)
            InvalidateArguments();
    }
}

extern "C" void FABLE_FASTCALL
FableDefTableCharStringDestroy_009D4C30(void*, void*)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactDefinitionTableStringDestroyCalls;
}

extern "C" void FABLE_FASTCALL
FableDefTableFileBaseDestroy_009D4C30(void* file, void*)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableFileDestroyCalls;
        if (file != g_ExactFile)
            InvalidateArguments();
    }
}

void FABLE_FASTCALL FableRunExactDefinitionTableSetPathBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.exactDefinitionTableSetPathCalls;
    CWideString recoveredPath(pathname != 0 ? pathname : L"");
    reinterpret_cast<CDefStringTableRecovered*>(
        &g_DefinitionTable)->SetTablePath(recoveredPath);
    state.exactDefinitionTablePathAssigned =
        wcscmp(
            static_cast<const wchar_t*>(g_DefinitionTable.tablePath_),
            pathname != 0 ? pathname : L"") == 0;
}

void FABLE_FASTCALL FableRunExactDefinitionTableLoadBoundary(
    bool readOnly,
    GFMainPhase6State& state)
{
    ++state.exactDefinitionTableLoadCalls;
    state.exactDefinitionTableArgumentsValid = true;
    state.exactDefinitionTableLoadedBeforeCleanup = false;
    g_DefinitionTable.loaded_ = false;
    g_DefinitionTable.readOnly_ = false;
    g_ExactFile = 0;
    g_ExactStream = 0;
    g_ActivePhase6State = &state;
    g_DefinitionTable.LoadTable(readOnly);
    g_ActivePhase6State = 0;
    state.exactDefinitionTableLoaded = g_DefinitionTable.loaded_;
    state.exactDefinitionTableReadOnly = g_DefinitionTable.readOnly_;
}

fable_u32 FABLE_FASTCALL FableRunExactWritePermissionsBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.exactPermissionProbeCalls;
    state.exactPermissionArgumentsValid = true;
    const wchar_t* source = pathname != 0 ? pathname : L"";
    wcsncpy(
        g_ExactPermissionProbePath,
        source,
        sizeof(g_ExactPermissionProbePath) /
            sizeof(g_ExactPermissionProbePath[0]) - 1);
    g_ExactPermissionProbePath[
        sizeof(g_ExactPermissionProbePath) /
            sizeof(g_ExactPermissionProbePath[0]) - 1] = L'\0';
    const size_t length = wcslen(g_ExactPermissionProbePath);
    if (
        length + 14 <
        sizeof(g_ExactPermissionProbePath) /
            sizeof(g_ExactPermissionProbePath[0]))
    {
        wcscat(g_ExactPermissionProbePath, L"write-test.tmp");
    }

    CWideString recoveredPath(source);
    g_ExactPermissionPath = &recoveredPath;
    const wchar_t* recoveredText =
        static_cast<const wchar_t*>(recoveredPath);
    const size_t recoveredLength = wcslen(recoveredText);
    g_ExactPermissionPrefix.begin = recoveredText;
    g_ExactPermissionPrefix.end =
        recoveredText + (recoveredLength < 2 ? recoveredLength : 2);
    g_ExactPermissionError.text = 0;
    g_ExactPermissionError.length = 0;
    g_ActivePhase6State = &state;
    const fable_u32 result =
        FableCheckMyDocumentsWritePermissions_00402130(recoveredPath);
    g_ActivePhase6State = 0;
    g_ExactPermissionPath = 0;
    state.exactPermissionResult = result;
    return result;
}
