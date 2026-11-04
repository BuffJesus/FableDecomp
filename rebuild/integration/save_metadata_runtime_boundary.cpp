#include "fable_filesystem.h"
#include "fable_game.h"
#include "fable_gfmain.h"
#include "fable_memory_file.h"
#include "fable_resource_directories.h"

// Stand-ins for the engine members CMainGameComponent::GenerateMetFilesFromLugFiles
// (0x00418C3B) reaches. The sound directory scan finds no .lug files, so the
// memory-file and lip-sync path is linked but not taken.

// Retail layout not recovered; only CAFile::GetFileInformation's return needs it.
class CFileInformation
{
};

namespace
{
    GFMainPhase9State* g_ActiveSaveMetadataState;
}

// NResourceDirectoryNames::GetSoundDir @ 0x0041A2B0: no sound directory here.
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetSoundDir()
{
    return CWideString();
}

// CAFile::StartFileScan @ 0x00999760: records the scan, finds nothing.
bool FABLE_FASTCALL CAFile::StartFileScan(
    const CWideString&,
    CFileScanInfo& info,
    wchar_t* pathname)
{
    if (g_ActiveSaveMetadataState != 0)
        ++g_ActiveSaveMetadataState->exactSaveMetadataFindFirstCalls;
    info.Stopped = true;
    info.SearchHandle = -1;
    if (pathname != 0)
        pathname[0] = L'\0';
    return false;
}

// CAFile::ScanNextFile @ 0x00999850.
bool FABLE_FASTCALL CAFile::ScanNextFile(
    const CWideString&,
    CFileScanInfo&,
    wchar_t* pathname)
{
    if (pathname != 0)
        pathname[0] = L'\0';
    return false;
}

// CAFile::SplitPath @ 0x00997620 (not reached: the scan finds nothing).
void FABLE_FASTCALL CAFile::SplitPath(
    const CWideString&,
    CWideString*,
    CWideString*,
    CWideString*,
    CWideString*)
{
}

// CBaseClassNonCopyable / CAFile base lifetime, needed by CMemoryFile's ctor.
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

CAFile::CAFile()
{
}

CFileInformation CAFile::GetFileInformation()
{
    return CFileInformation();
}

// CMemoryFile (retail ctor 0x009E97B0, DumpToFile 0x009E9330): an in-memory
// file whose dump writes nothing in this runtime.
CMemoryFile::CMemoryFile(
    const CWideString&,
    FileAccessType accessType,
    unsigned long)
    : AccessType(accessType),
      Position(0),
      Length(0),
      Opened(true)
{
}

void CMemoryFile::DumpToFile()
{
}

void CMemoryFile::Open(const CWideString&, FileAccessType, unsigned long) {}
void CMemoryFile::Close() {}
void CMemoryFile::Read(void*, long, CASuspendableProcess*) {}
void CMemoryFile::Write(const void*, long, CASuspendableProcess*) {}
void CMemoryFile::SetPosition(long) {}
void CMemoryFile::MovePosition(long) {}
long CMemoryFile::GetPosition() const { return 0; }
void CMemoryFile::SetLength(long) {}
long CMemoryFile::GetLength() const { return 0; }
bool CMemoryFile::IsOpen() const { return Opened; }
CAFile* CMemoryFile::Clone() const { return 0; }

void FABLE_FASTCALL FableRunExactSaveMetadataBoundary(
    GFMainPhase9State& state)
{
    ++state.exactSaveMetadataCoordinatorCalls;
    g_ActiveSaveMetadataState = &state;
    CMainGameComponent::GenerateMetFilesFromLugFiles();
    g_ActiveSaveMetadataState = 0;
}
