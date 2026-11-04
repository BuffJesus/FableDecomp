#pragma once

#include "fable_base_class.h"
#include "fable_string.h"

class CASuspendableProcess;
class CFileInformation;
class CDateAndTime;

// Retail CDiskFileWin32::Open switches on 1..4. Enumerators: FableWin.pdb
// enum FileAccessType.
enum FileAccessType
{
    FILE_ACCESS_NULL = 0,
    FILE_ACCESS_READ = 1,
    FILE_ACCESS_READ_WRITE = 2,
    FILE_ACCESS_READ_WRITE_CREATE = 3,
    FILE_ACCESS_CREATE = 4
};

// RTTI: CAFile : CBaseClassNonCopyable : CBaseClass. Slot order from the
// retail CDiskFileWin32 vtable (0x122D06C).
// Retail omits CAFile vptr writes in construction and destruction.
class __declspec(novtable) CAFile : public CBaseClassNonCopyable
{
public:
    CAFile();
    virtual ~CAFile() {}

    virtual void Open(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags) = 0;
    virtual void Close() = 0;
    virtual void Read(
        void* data,
        long size,
        CASuspendableProcess* process) = 0;
    virtual void Write(
        const void* data,
        long size,
        CASuspendableProcess* process) = 0;
    virtual void SetPosition(long position) = 0;
    virtual void MovePosition(long offset) = 0;
    virtual long GetPosition() const = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;
    virtual bool IsOpen() const = 0;
    virtual CWideString GetPathname() const = 0;
    virtual bool IsIOFinished() const { return true; }
    virtual long GetAlignment() const { return 1; }
    virtual CAFile* Clone() const = 0;
    virtual CFileInformation GetFileInformation();

    static CWideString FABLE_FASTCALL GetCurrentPath();
    // Ego_r, retail 0x009975B0 / 0x009975C0 (CAFile_PathHelpers_009975b0.cpp).
    static long FABLE_FASTCALL GetHDSectorSize();
    static long FABLE_FASTCALL GetDVDSectorSize();
    static CWideString FABLE_FASTCALL ExpandPath(
        const CWideString& pathname,
        const CWideString& context);
    static CDateAndTime FABLE_FASTCALL GetFileModifiedTime(const CWideString& pathname);
    static bool FABLE_FASTCALL PathExists(const CWideString& pathname);
    // FableWin ?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z, retail
    // 0x00998C30 (ExpandPath 0x00998C90 resolves "..\" paths through it).
    static CWideString FABLE_FASTCALL GetAbsoluteFilePath(
        const CWideString& directory,
        const CWideString& path);
    // Ego_r ?GetCopyFilename@CAFile@@SI?AVCCharString@@ABV2@@Z, retail 0x00997BB0.
    static CCharString FABLE_FASTCALL GetCopyFilename(const CCharString& old_name);
    // Ego_r ?GetRelativeFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z, retail 0x00998570.
    static CCharString FABLE_FASTCALL GetRelativeFilePath(
        const CCharString& path1_absolute,
        const CCharString& path2_absolute);
    // Ego_r ?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z, retail 0x00998250.
    static CCharString FABLE_FASTCALL GetAbsoluteFilePath(
        const CCharString& directory,
        const CCharString& path);
    static void FABLE_FASTCALL SetCurrentPath(const CWideString& path);
    // ?CopyFileW@CAFile@@SIXAAV1@0@Z, retail 0x00998E70 (the source's
    // CopyFile went through <windows.h>'s CopyFile -> CopyFileW macro).
    static void FABLE_FASTCALL CopyFileW(CAFile& source, CAFile& destination);
    static CWideString FABLE_FASTCALL GetProjectPath();
    static CWideString FABLE_FASTCALL ConvertPathToFullPath(
        const CWideString& path);
    // Ego_r ?ConvertPathToFullPath@CAFile@@SI?AVCCharString@@ABV2@@Z, retail 0x009990B0.
    static CCharString FABLE_FASTCALL ConvertPathToFullPath(const CCharString& path);
    // Ego_r ?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile@@SI_KABVCWideString@@@Z,
    // retail 0x00999320; ?IsFileReadOnly@CAFile@@SI_NABVCWideString@@@Z, retail 0x009993B0.
    static unsigned __int64 FABLE_FASTCALL GetFileModifiedTimeAsSomeWackyWin32BigNumber(
        const CWideString& pathname);
    static bool FABLE_FASTCALL IsFileReadOnly(const CWideString& pathname);
    // Ego_r ?IsPathFile@CAFile@@SI_NABVCWideString@@@Z, retail 0x00999A30.
    static bool FABLE_FASTCALL IsPathFile(const CWideString& pathname);
    // Ego_r ?SetFileReadOnly@CAFile@@SIXABVCWideString@@_N@Z, retail 0x00999420.
    static void FABLE_FASTCALL SetFileReadOnly(const CWideString& pathname, bool read_only);
    // FableWin ?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z,
    // retail 0x00999140 (CDiskFileWin32::Open 0x0099A6A0 ignores the result).
    static bool FABLE_FASTCALL ConvertPathToFullPath(
        const CWideString& path,
        CWideString& fullPath);
    // FableWin ?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z,
    // retail 0x00999970; ?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z,
    // retail 0x00999620 (both called by CDiskFileWin32::Open's write modes).
    static void FABLE_FASTCALL ValidatePathnameIsOnWriteableDrive(
        const CWideString& pathname);
    static void FABLE_FASTCALL CreateDirectoryForFile(
        const CWideString& pathname);
    // Ego_r ?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z, retail 0x009994A0
    // (the source's CreateDirectory went through the windows.h macro);
    // ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z, retail 0x00999AB0.
    static void FABLE_FASTCALL CreateDirectoryW(const CWideString& pathname);
    static bool FABLE_FASTCALL IsRoomForFile(long file_size, const CWideString& dst_path);
    // Ego_r ?CompareFiles@CAFile@@SI_NAAV1@0@Z, retail 0x00998F60.
    static bool FABLE_FASTCALL CompareFiles(CAFile& src_file, CAFile& dst_file);
    static void FABLE_FASTCALL SplitPath(
        const CWideString& path,
        CWideString* drive,
        CWideString* directory,
        CWideString* filename,
        CWideString* extension);
    // Ego_r ?ConvertDirectoryAndFilenameToPath@CAFile@@SI?AVCWideString@@ABV2@0@Z,
    // retail 0x009979E0.
    static CWideString FABLE_FASTCALL ConvertDirectoryAndFilenameToPath(
        const CWideString& directory,
        const CWideString& filename);
    // Ego_r ?GetFilenameFromPathname@CAFile@@SI?AVCWideString@@ABV2@@Z, retail 0x00997930.
    static CWideString FABLE_FASTCALL GetFilenameFromPathname(const CWideString& pathname);
    // Ego_r ?IsFilePathSyntaxValid@CAFile@@SI_NABVCWideString@@_N@Z, retail 0x00998A30.
    static bool FABLE_FASTCALL IsFilePathSyntaxValid(const CWideString& pathname_wide, bool absolute_path);
    // Ego_r ?SetFilenameExtension@CAFile@@SI?AVCWideString@@ABV2@0@Z, retail 0x00997800.
    static CWideString FABLE_FASTCALL SetFilenameExtension(
        const CWideString& pathname,
        const CWideString& ext);
    // Ego_r ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z, retail 0x00997780.
    static CWideString FABLE_FASTCALL MakePath(
        const CWideString& drive,
        const CWideString& directory,
        const CWideString& filename,
        const CWideString& extension);
    // Ego_r ?SplitPath@CAFile@@SIXABVCCharString@@PAV2@111@Z, retail 0x009976D0.
    static void FABLE_FASTCALL SplitPath(
        const CCharString& path,
        CCharString* drive,
        CCharString* directory,
        CCharString* filename,
        CCharString* extension);

    class CFileScanInfo
    {
    public:
        bool Stopped;
        long SearchHandle;
    };

    static bool FABLE_FASTCALL StartFileScan(
        const CWideString& pattern,
        CFileScanInfo& info,
        wchar_t* pathname);
    static bool FABLE_FASTCALL ScanNextFile(
        const CWideString& pattern,
        CFileScanInfo& info,
        wchar_t* pathname);
    // The CWideString overloads (Ego_r ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z
    // and ?ScanNextFile@...), retail 0x00999C50 / 0x00999CB0
    // (CDefinitionManager::CheckDependencies 0x009B05F0 calls them).
    static bool FABLE_FASTCALL StartFileScan(
        const CWideString& pattern,
        CFileScanInfo& info,
        CWideString& pathname);
    static bool FABLE_FASTCALL ScanNextFile(
        const CWideString& pattern,
        CFileScanInfo& info,
        CWideString& pathname);

    // Ego_r.pdb static data (FableWin setters ?SetErrorCallback@CAFile@@SIXP6IXXZ@Z,
    // ?SetHardDiskErrorCallback@CAFile@@SIXP6IXPBD@Z@Z). Retail keeps
    // m_harddiskErrorCallback at 0x013BCA00 (CDiskFileWin32::Write 0x0099AA90
    // tests and calls it inline).
private:
    // Ego_r private statics, retail 0x009975D0 / 0x009975E0.
    static bool FABLE_FASTCALL ParserIsFinished(class CStringParser& parser);
    // Ego_r CAFile::CExpectedTextType (4 bools): what IsFilePathSyntaxValid may meet next.
    struct CExpectedTextType
    {
        void Set(bool component_ok, bool separator_ok, bool drive_label_ok, bool finished_ok)
        {
            ComponentOK = component_ok;
            SeparatorOK = separator_ok;
            DriveLabelOK = drive_label_ok;
            FinishedOK = finished_ok;
        }
        bool ComponentOK;
        bool SeparatorOK;
        bool DriveLabelOK;
        bool FinishedOK;
    };
    // Ego_r private statics, retail 0x00997DA0 / 0x00997E90.
    static bool FABLE_FASTCALL ParserIsSeparatorNext(class CStringParser& parser);
    static bool FABLE_FASTCALL ParserReadPastSeparator(class CStringParser& parser);
    // Ego_r private statics, retail 0x00998170 / 0x009981D0 / 0x00998210 / 0x00998010.
    static bool FABLE_FASTCALL ParserReadPastDriveLabel(class CStringParser& parser, class CParsedItem& item);
    static bool FABLE_FASTCALL ParserIsComponentNext(class CStringParser& parser);
    static bool FABLE_FASTCALL ParserIsDriveLabelNext(class CStringParser& parser);
    static bool FABLE_FASTCALL ParserReadPastComponent(class CStringParser& parser, CCharString& component);
    static bool FABLE_FASTCALL IsForbiddenSymbolInComponent(const CCharString& component);

public:
    static void (FABLE_FASTCALL* m_errorCallback)();
    static void (FABLE_FASTCALL* m_harddiskErrorCallback)(const char* message);
    // Ego_r CAFile::DebugUseReadOnlyDrives, retail 0x013BC9F8 (set by
    // SetDebugUseReadOnlyDrives 0x00999950; ValidatePathnameIsOnWriteableDrive
    // 0x00999970 tests it).
    static bool DebugUseReadOnlyDrives;

    // Inline in retail (CDataInputStream::ReadCompressed 0x00995FB0:
    // `mov eax,[0x13bc9fc]; test; call eax`); m_errorCallback is 0x013BC9FC
    // (setter 0x00401600, getter 0x00993AD0).
    static void DoErrorCallback()
    {
        if (m_errorCallback != 0)
            m_errorCallback();
    }

    // Inline in retail (Write 0x0099AA90: `mov eax,[0x13bca00]; test; call eax`).
    static void DoHardDiskErrorCallback(const char* message)
    {
        if (m_harddiskErrorCallback != 0)
            m_harddiskErrorCallback(message);
    }
};

// Retail 0x00999110 (out of line, global_ConvertPathToFullPath_00999110.cpp);
// ValidatePathnameIsOnWriteableDrive 0x00999970 expands it in place, so a
// TU that needs that defines FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE.
#ifdef FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE
inline CWideString FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CWideString& path)
{
    return ExpandPath(path, GetCurrentPath());
}
#endif

// CAFile's process-wide working-directory cache. The address suffix is kept
// until the enclosing retail static-data layout has been fully recovered.
extern CWideString g_CAFileCurrentPath_013BCA10;

namespace NFile
{
// Ego_r ?CalcCurrentPath@NFile@@YI?AVCWideString@@XZ, retail 0x009974A0: the
// process working directory (out of line in retail; CAFile::GetCurrentPath
// 0x00997F80 calls it the first time).
CWideString FABLE_FASTCALL CalcCurrentPath();
// Ego_r ?ConvertDriveLetterToNumber@NFile@@YIJ_W@Z, retail 0x00997470: a = 1.
long FABLE_FASTCALL ConvertDriveLetterToNumber(wchar_t letter);
// Ego_r ?GetItemFromPath@NFile@@YI_NPB_WKPA_W@Z, retail 0x00997880.
bool FABLE_FASTCALL GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
// Ego_r ?GetItemFromPath@NFile@@YI_NABVCWideString@@KAAV2@@Z, retail 0x00997B40.
bool FABLE_FASTCALL GetItemFromPath(const CWideString& pathname, unsigned long directory_number, CWideString& directory_name);
}
