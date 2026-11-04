// MainGameComponent.cpp is size-optimised in retail (see the 0x42EA8F ctor).
#pragma optimize("s", on)

#include "fable_filesystem.h"
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_memory_file.h"
#include "fable_resource_directories.h"
#include "fable_sound_manager.h"

// RTTI: CBinaryFileDataWriter : LHA::CBinaryDataWriter. Vtable 0x122ED7C; the
// out-of-line dtor and Write follow CMainGameComponent's ctor at 0x418EF0.
class CBinaryFileDataWriter : public LHA::CBinaryDataWriter
{
public:
    CBinaryFileDataWriter(CAFile* file)
        : File(file)
    {
    }

    ~CBinaryFileDataWriter()
    {
    }

    virtual bool Write(const void* data, long size)
    {
        File->Write(data, size, 0);
        return true;
    }

    CAFile* File;
};

// CMainGameComponent::GenerateMetFilesFromLugFiles() @ 0x00418C3B.
void FABLE_FASTCALL CMainGameComponent::GenerateMetFilesFromLugFiles()
{
    CWideString searchPattern =
        NResourceDirectoryNames::GetSoundDir() + L"*.lug";
    CAFile::CFileScanInfo scanInfo;
    wchar_t pathname[260 + 1];

    bool found = CAFile::StartFileScan(searchPattern, scanInfo, pathname);
    while (found)
    {
        CWideString filename;
        CWideString extension;
        {
            CWideString foundPathname(pathname);
            CAFile::SplitPath(foundPathname, 0, 0, &filename, &extension);
        }

        CMemoryFile metFile(
            NResourceDirectoryNames::GetSoundDir() + filename + L".met",
            (FileAccessType)3,
            2);
        CBinaryFileDataWriter writer(&metFile);
        GFGetSystemManager()->PSoundManager.Data->GenerateLipSyncData(
            NResourceDirectoryNames::GetSoundDir() + filename + extension,
            writer);
        metFile.DumpToFile();

        found = CAFile::ScanNextFile(searchPattern, scanInfo, pathname);
    }
}
