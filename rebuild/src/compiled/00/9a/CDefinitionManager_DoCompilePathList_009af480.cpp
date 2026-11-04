// CDefinitionManager::DoCompilePathList @ 0x009AF480.
// Scan and sort each path pattern before loading definitions from its files.
#include <stdlib.h>
#include <new>
#include "fable_definition_manager.h"
#include "fable_filesystem.h"
#include "fable_disk_file.h"
#include "fable_comments.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_algo.h"

void CDefinitionManager::DoCompilePathList(
    const _STL::vector<CWideString>& pathList, const CCharString& prefix)
{
    if (pathList.begin() == pathList.end())
        return;

    for (const CWideString* pattern = pathList.begin(); pattern != pathList.end(); ++pattern)
    {
        _STL::vector<CWideString> files;
        CWideString pathname;
        CCharString trace("DefinitionManager : DoCompilePathList");
        CAFile::CFileScanInfo scanInfo;
        if (CAFile::StartFileScan(*pattern, scanInfo, pathname))
        {
            do
            {
                files.push_back(pathname.ToCharString().GetAsLowerCase().ToWideString());
            } while (CAFile::ScanNextFile(*pattern, scanInfo, pathname));
        }
        _STL::sort(files.begin(), files.end(), _STL::less<CWideString>());

        for (const CWideString* it = files.begin(); it != files.end(); ++it)
        {
            CDiskFileWin32 file(*it, FILE_ACCESS_READ, 2);
            long length = file.GetLength();
            char* buffer = static_cast<char*>(malloc(length + 1));
            file.Read(buffer, length, 0);
            buffer[length] = 0;
            GFRemoveComments(buffer, "//", "\n");
            GFRemoveComments(buffer, "//", "\r");
            GFRemoveComments(buffer, "/*", "*/");
            CurrentFilename = CWideString(*it).ToCharString();
            CompileString(buffer, prefix);
            free(buffer);
        }
    }
}
