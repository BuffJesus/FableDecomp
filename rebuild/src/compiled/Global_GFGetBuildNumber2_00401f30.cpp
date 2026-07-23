#include "candidates/build_number_compiled.h"

void FABLE_CDECL GFGetBuildNumber2()
{
    CCharString pathText;
    CCharString fileContents;
    CDiskFileWin32 diskFile;

    pathText.AssignFromWide(L"full_build_version.txt");
    const bool pathExists = reinterpret_cast<const CWideString*>(&pathText)->PathExists();
    pathText.Destroy();

    if (pathExists)
    {
        pathText.AssignFromWide(L"full_build_version.txt");
        diskFile.Construct(*reinterpret_cast<const CWideString*>(&pathText), 1, 2);
        pathText.Destroy();

        fileContents.ConstructFromFile(reinterpret_cast<CAFile*>(&diskFile), -1);
        FableFullBuildVersion.Assign(fileContents);
        fileContents.Destroy();

        diskFile.Vftable = FableDiskFileWin32Vftable;
        diskFile.Close();
        diskFile.Path.Destroy();
        diskFile.BaseCleanup();
    }
}
