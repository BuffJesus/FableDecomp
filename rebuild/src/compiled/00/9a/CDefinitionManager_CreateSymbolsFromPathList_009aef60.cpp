// CDefinitionManager::CreateSymbolsFromPathList @ 0x009AEF60.
// Enumerate each symbol path pattern, normalize the resulting filenames,
// parse them in sorted order, then publish dirty symbol-map entries.
#include <stdlib.h>
#include <new>
#include "fable_definition_manager.h"
#include "fable_filesystem.h"
#include "fable_disk_file.h"
#include "fable_comments.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_algo.h"

void CDefinitionManager::CreateSymbolsFromPathList(
    const _STL::vector<CWideString>& pathList, bool flag)
{
    CCharString trace("DefinitionManager : CreateSymbolsFromPathList");
    for (const CWideString* pattern = pathList.begin(); pattern != pathList.end(); ++pattern)
    {
        _STL::vector<CWideString> files;
        CWideString pathname;
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
            ParseStringForSymbols(buffer, flag);
            free(buffer);
        }
    }
    PSymbolMap->UpdateDirty();
    PDefinitionSymbolMap->UpdateDirty();
}
