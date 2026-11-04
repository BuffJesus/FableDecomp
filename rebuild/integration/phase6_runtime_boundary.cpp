// Exact Phase 6 ownership routes used by complete and visual boot.
#include "fable_data_input_stream.h"
#include "fable_definition_table.h"
#include "fable_disk_file.h"
#include "fable_gfmain.h"
#include "fable_write_permissions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// kernel32, hand-declared (the integration units link no windows.h).
extern "C" __declspec(dllimport) int FABLE_STDCALL DeleteFileW(
    const wchar_t* fileName);
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL GetTempPathW(
    unsigned long bufferLength,
    wchar_t* buffer);
extern "C" __declspec(dllimport) int FABLE_STDCALL CreateDirectoryW(
    const wchar_t* pathName,
    void* securityAttributes);
extern "C" __declspec(dllimport) int FABLE_STDCALL RemoveDirectoryW(
    const wchar_t* pathName);
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL
GetFileAttributesW(const wchar_t* fileName);
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL
GetCurrentProcessId();
extern "C" __declspec(dllimport) void* FABLE_STDCALL GetCurrentProcess();
extern "C" __declspec(dllimport) int FABLE_STDCALL DuplicateHandle(
    void* sourceProcess,
    void* sourceHandle,
    void* targetProcess,
    void** targetHandle,
    unsigned long desiredAccess,
    int inheritHandle,
    unsigned long options);
extern "C" __declspec(dllimport) int FABLE_STDCALL GetHandleInformation(
    void* object,
    unsigned long* flags);
extern "C" __declspec(dllimport) int FABLE_STDCALL CloseHandle(void* object);

// Only CAFile::GetFileInformation's return type needs it.
class CFileInformation
{
};

namespace
{
    // The authored boundary needs only filename ownership and the two flags.
    // Keep that storage explicit instead of constructing the native map/stream.
    struct DefinitionTableBoundaryStorage
    {
        unsigned long reserved[12];
        CWideString filename;
        bool locked;
        bool readOnly;
    };
    FABLE_STATIC_ASSERT(sizeof(DefinitionTableBoundaryStorage) == sizeof(CDefStringTable));
    FABLE_STATIC_ASSERT(offsetof(DefinitionTableBoundaryStorage, filename) == 0x30);
    DefinitionTableBoundaryStorage g_DefinitionTableStorage;
    CDefStringTable& g_DefinitionTable =
        *reinterpret_cast<CDefStringTable*>(&g_DefinitionTableStorage);
    GFMainPhase6State* g_ActivePhase6State;
    const unsigned long kDuplicateSameAccess = 2;

    CDiskFileWin32* g_ExactFile;
    void* g_ExactStream;
    void* g_ExactFileHandle;
    fable_i32 g_ExactWideStringsAtOpen;
    bool g_ExactFileOpenAtStreamDestroy;

    // 0x00402130 appends this to the folder it probes.
    const wchar_t kProbeSuffix[] =
        L"This directory has restricted permissions, and Fable cannot run properly";

    const CWideString* g_ExactPermissionPath;
    wchar_t g_ExactPermissionProbeDirectory[260];
    wchar_t g_ExactPermissionProbePath[400];
    int g_ExactPermissionHandle;
    bool g_ExactPermissionDeleted;

    void InvalidateArguments()
    {
        if (g_ActivePhase6State != 0)
            g_ActivePhase6State->exactDefinitionTableArgumentsValid = false;
    }

    void InvalidatePermissionArguments()
    {
        if (g_ActivePhase6State != 0)
            g_ActivePhase6State->exactPermissionArgumentsValid = false;
    }

    bool IsProbePath(const wchar_t* filename)
    {
        return filename != 0 &&
            wcscmp(filename, g_ExactPermissionProbePath) == 0;
    }
}

// CWideString::GetSubString @ 0x0099BDB0. Retail treats the second argument
// as the end index (0x00402130 asks for (0, 2), the first two characters).
// The data is the first/last/end vector the inline operator== reads.
CWideString CWideString::GetSubString(
    unsigned long start,
    unsigned long count) const
{
    if (
        g_ActivePhase6State != 0 &&
        (this != g_ExactPermissionPath || start != 0 || count != 2))
    {
        InvalidatePermissionArguments();
    }

    CWideString result;
    const wchar_t* base =
        storage_ != 0 ? storage_->text : g_FableEmptyWideString_0129A8E0;
    const wchar_t* text = base + start;
    if (*text != L'\0')
    {
        size_t length = count > start ? count - start : 0;
        const size_t available = wcslen(text);
        if (length > available)
            length = available;

        CWideStringData* data = static_cast<CWideStringData*>(
            operator new(sizeof(CWideStringData)));
        data->text = static_cast<wchar_t*>(
            malloc((length + 1) * sizeof(wchar_t)));
        memcpy(data->text, text, length * sizeof(wchar_t));
        data->text[length] = L'\0';
        data->unknown04 =
            static_cast<fable_u32>(reinterpret_cast<size_t>(
                data->text + length));
        data->unknown08 =
            static_cast<fable_u32>(reinterpret_cast<size_t>(
                data->text + length + 1));
        data->owners = 1;
        result.storage_ = data;
    }
    return result;
}

// CCharString::operator!=(const char*) @ 0x0099E960.
bool CCharString::operator!=(const char* other) const
{
    return strcmp(GetConstCharPtr(), other) != 0;
}

// kernel32 import slots 0x0143FE2C/30/34. CreateFileW and CloseHandle are
// recorded without touching the disk, as before; the probe file itself is now
// created by the CRT's _wfopen, so DeleteFileW really removes it.
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
        !IsProbePath(filename) ||
        desiredAccess != 0x10000000 ||
        shareMode != 3 ||
        creationDisposition != 2 ||
        flags != 0)
    {
        InvalidatePermissionArguments();
    }
    return &g_ExactPermissionHandle;
}

extern "C" int FABLE_STDCALL
FableWriteCloseHandleStub_00402130(void* handle)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionCloseHandleCalls;
    if (handle != &g_ExactPermissionHandle)
        InvalidatePermissionArguments();
    return 1;
}

extern "C" int FABLE_STDCALL
FableWriteDeleteFileStub_00402130(const wchar_t* filename)
{
    if (g_ActivePhase6State != 0)
        ++g_ActivePhase6State->exactPermissionDeleteCalls;
    if (!IsProbePath(filename))
    {
        InvalidatePermissionArguments();
        return 0;
    }
    g_ExactPermissionDeleted = DeleteFileW(filename) != 0;
    return g_ExactPermissionDeleted ? 1 : 0;
}

extern "C" void* g_FableCreateFileW_0143FE2C =
    reinterpret_cast<void*>(&FableWriteCreateFileStub_00402130);
extern "C" void* g_FableCloseHandle_0143FE30 =
    reinterpret_cast<void*>(&FableWriteCloseHandleStub_00402130);
extern "C" void* g_FableDeleteFileW_0143FE34 =
    reinterpret_cast<void*>(&FableWriteDeleteFileStub_00402130);

// CDefStringTable::LoadTable (0x009D4C30) is genuine: it calls the real
// members below and emits the CDiskFileWin32 vftable, so every virtual must
// link. CDiskFileWin32::Close (0x0099A920) is the landed reconstruction;
// CAFile's own lifetime comes from save_metadata_runtime_boundary.cpp and
// CWideString's from the landed 0x0099AED0/0x0099B510 objects. The rest are
// stand-ins that keep the old adapters' behaviour: the table path is a
// placeholder, so nothing is read from disk.

// CAFile::PathExists @ 0x00999230 (landed, but it expands the path against
// the working directory, and the placeholder path does not exist).
bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTablePathExistsCalls;
        if (&pathname != &g_DefinitionTable.TableFilename)
            InvalidateArguments();
    }
    return true;
}

// CDiskFileWin32::Open @ 0x0099A6A0: opens nothing. It hands the real Close
// a live handle (a duplicate of the process pseudo-handle), so the runtime
// can see that Close really released it.
void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    AccessType = accessType;
    Position = 0;
    Length = 0;
    if (g_ActivePhase6State == 0)
        return;

    ++g_ActivePhase6State->exactDefinitionTableOpenCalls;
    g_ExactFile = this;
    g_ExactWideStringsAtOpen = g_CWideStringInstanceCount_013BCA20;
    if (
        IsOpen() ||
        &pathname != &g_DefinitionTable.TableFilename ||
        accessType != 1 ||
        flags != 4)
    {
        InvalidateArguments();
    }

    void* const process = GetCurrentProcess();
    g_ExactFileHandle = 0;
    if (DuplicateHandle(
            process, process, process, &g_ExactFileHandle, 0, 0,
            kDuplicateSameAccess) == 0)
    {
        g_ExactFileHandle = 0;
        InvalidateArguments();
        return;
    }
    WinFileHandle = g_ExactFileHandle;
    Opened = true;
}

// CDiskFileWin32 Read @ 0x0099A960, Write, SetPosition, MovePosition,
// GetPosition @ 0x0099ABD0, SetLength, Clone, GetFileInformation: vftable
// entries LoadTable never reaches here.
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long position) { Position = position; }
void CDiskFileWin32::MovePosition(long offset) { Position += offset; }
long CDiskFileWin32::GetPosition() const { return Position; }
void CDiskFileWin32::SetLength(long length) { Length = length; }
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation()
{
    return CFileInformation();
}

// CDataInputStream::SetPosition @ 0x00993BC0 (a vftable entry LoadTable never
// reaches here). ~CDataStream / ~CDataInputStream are inline in the header.
void CDataInputStream::SetPosition(unsigned long position)
{
    StreamPos = position;
}

// CDataInputStream::CDataInputStream @ 0x00993B20 (out of line): every
// field zero; the file stream constructor below zeroes them again.
CDataInputStream::CDataInputStream()
{
    StreamPos = 0;
    StreamSize = 0;
    CurrentSourcePtr = 0;
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = 0;
}

// CFileDataInputStream::CFileDataInputStream @ 0x00994700: no buffer.
CFileDataInputStream::CFileDataInputStream(
    CAFile* file,
    unsigned long bufferSize)
{
    StreamPos = 0;
    StreamSize = 0;
    CurrentSourcePtr = 0;
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = 0;
    File = file;
    Buffer = 0;
    BufferSize = 0;
    g_ExactStream = this;
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableStreamConstructCalls;
        if (file != g_ExactFile || bufferSize != 0x4000)
            InvalidateArguments();
    }
}

// CFileDataInputStream::~CFileDataInputStream @ 0x00994780.
CFileDataInputStream::~CFileDataInputStream()
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableStreamDestroyCalls;
        g_ActivePhase6State->exactDefinitionTableLoadedBeforeCleanup =
            g_DefinitionTable.Locked;
        if (this != g_ExactStream)
            InvalidateArguments();
        // The file is still open while the stream goes away.
        g_ExactFileOpenAtStreamDestroy =
            g_ExactFile != 0 && g_ExactFile->IsOpen();
    }
}

// CDefStringTable::ReadTable @ 0x009D4AB0: the placeholder table is empty.
void CDefStringTable::ReadTable(CDataInputStream& stream)
{
    if (g_ActivePhase6State != 0)
    {
        ++g_ActivePhase6State->exactDefinitionTableReadCalls;
        if (this != &g_DefinitionTable || &stream != g_ExactStream)
            InvalidateArguments();
    }
}

void FABLE_FASTCALL FableRunExactDefinitionTableSetPathBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.exactDefinitionTableSetPathCalls;
    CWideString recoveredPath(pathname != 0 ? pathname : L"");
    g_DefinitionTable.SetTableFilename(recoveredPath);
    state.exactDefinitionTablePathAssigned =
        wcscmp(
            static_cast<const wchar_t*>(g_DefinitionTable.TableFilename),
            pathname != 0 ? pathname : L"") == 0;
}

void FABLE_FASTCALL FableRunExactDefinitionTableLoadBoundary(
    bool readOnly,
    GFMainPhase6State& state)
{
    ++state.exactDefinitionTableLoadCalls;
    state.exactDefinitionTableArgumentsValid = true;
    state.exactDefinitionTableLoadedBeforeCleanup = false;
    g_DefinitionTable.Locked = false;
    g_DefinitionTable.ReadOnly = false;
    g_ExactFile = 0;
    g_ExactStream = 0;
    g_ExactFileHandle = 0;
    g_ExactFileOpenAtStreamDestroy = false;
    const fable_i32 wideBefore = g_CWideStringInstanceCount_013BCA20;
    g_ActivePhase6State = &state;
    g_DefinitionTable.LoadTable(readOnly);
    g_ActivePhase6State = 0;

    // The file's cleanup is real code (the inline ~CDiskFileWin32: Close
    // 0x0099A920, ~CWideString 0x0099B510, ~CAFile), so it is read from its
    // effects rather than counted: Close released Open's handle after the
    // stream was gone, and the file's Pathname string was destroyed. The
    // order and the ~CAFile call are pinned by the 0x009D4C30 fixture.
    unsigned long handleFlags = 0;
    const bool handleReleased =
        g_ExactFileHandle != 0 &&
        GetHandleInformation(g_ExactFileHandle, &handleFlags) == 0;
    if (!handleReleased && g_ExactFileHandle != 0)
        CloseHandle(g_ExactFileHandle);
    const bool fileCleanedUp =
        g_ExactFile != 0 &&
        g_ExactFileOpenAtStreamDestroy &&
        handleReleased &&
        g_ExactWideStringsAtOpen == wideBefore + 1 &&
        g_CWideStringInstanceCount_013BCA20 == wideBefore;
    if (fileCleanedUp)
    {
        ++state.exactDefinitionTableCloseCalls;
        ++state.exactDefinitionTableStringDestroyCalls;
        ++state.exactDefinitionTableFileDestroyCalls;
    }
    else
    {
        state.exactDefinitionTableArgumentsValid = false;
    }
    state.exactDefinitionTableLoaded = g_DefinitionTable.Locked;
    state.exactDefinitionTableReadOnly = g_DefinitionTable.ReadOnly;
}

// The caller's My Documents path is a placeholder here ("My Documents\\"),
// and 0x00402130 now creates and deletes a real probe file through the CRT,
// so the probe runs in a private temporary folder that is removed afterwards.
fable_u32 FABLE_FASTCALL FableRunExactWritePermissionsBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.exactPermissionProbeCalls;
    state.exactPermissionArgumentsValid = pathname != 0;
    state.exactPermissionProbeFileRemoved = false;

    const unsigned long directoryCapacity =
        sizeof(g_ExactPermissionProbeDirectory) /
            sizeof(g_ExactPermissionProbeDirectory[0]);
    if (GetTempPathW(directoryCapacity - 40,
            g_ExactPermissionProbeDirectory) == 0)
    {
        g_ExactPermissionProbeDirectory[0] = L'\0';
    }
    const size_t tempLength = wcslen(g_ExactPermissionProbeDirectory);
    _snwprintf(
        g_ExactPermissionProbeDirectory + tempLength,
        directoryCapacity - tempLength - 1,
        L"FableTLC-write-probe-%lu\\",
        GetCurrentProcessId());
    CreateDirectoryW(g_ExactPermissionProbeDirectory, 0);
    const size_t probeCapacity =
        sizeof(g_ExactPermissionProbePath) /
            sizeof(g_ExactPermissionProbePath[0]);
    _snwprintf(
        g_ExactPermissionProbePath,
        probeCapacity - 1,
        L"%s%s",
        g_ExactPermissionProbeDirectory,
        kProbeSuffix);
    g_ExactPermissionProbePath[probeCapacity - 1] = L'\0';
    g_ExactPermissionDeleted = false;

    const fable_i32 wideBefore = g_CWideStringInstanceCount_013BCA20;
    const fable_i32 charBefore = g_CCharStringInstanceCount_013BD800;
    fable_u32 result;
    {
        CWideString probeDirectory(g_ExactPermissionProbeDirectory);
        g_ExactPermissionPath = &probeDirectory;
        g_ActivePhase6State = &state;
        result = FableCheckMyDocumentsWritePermissions_00402130(
            probeDirectory);
        g_ActivePhase6State = 0;
        g_ExactPermissionPath = 0;
    }
    state.exactPermissionWideStringDelta =
        g_CWideStringInstanceCount_013BCA20 - wideBefore;
    state.exactPermissionCharStringDelta =
        g_CCharStringInstanceCount_013BD800 - charBefore;

    state.exactPermissionProbeFileRemoved =
        g_ExactPermissionDeleted &&
        GetFileAttributesW(g_ExactPermissionProbePath) == 0xFFFFFFFFu;
    RemoveDirectoryW(g_ExactPermissionProbeDirectory);
    state.exactPermissionResult = result;
    return result;
}

// The input streams' other vtable slots (the shared header models retail's
// vtables 0x129A6B0 / 0x129A728). LoadTable's path never calls them: the reads
// abort, the rest are the base stream's empty bodies.
void CDataInputStream::Close()
{
    StreamPos = 0;
    StreamSize = 0;
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = 0;
    CurrentSourcePtr = 0;
}
void CDataInputStream::ReadCompressed(void*, long) { abort(); }
void CDataInputStream::ReadZlibCompressed(void*, long) { abort(); }
CCharString CDataInputStream::ReadNullTerminatedString() { abort(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { abort(); }
void CDataInputStream::InternalReadNextSourceChunk(const fable_u8*&, long&) {}
bool CDataInputStream::NeedsBufferedRead(long) { return false; }
void CDataInputStream::InternalReadUnbuffered(fable_u8*, long) {}
void CFileDataInputStream::Close() { CDataInputStream::Close(); }
void CFileDataInputStream::InternalReadNextSourceChunk(const fable_u8*&, long&) { abort(); }
bool CFileDataInputStream::NeedsBufferedRead(long) { abort(); }
void CFileDataInputStream::InternalReadUnbuffered(fable_u8*, long) { abort(); }
