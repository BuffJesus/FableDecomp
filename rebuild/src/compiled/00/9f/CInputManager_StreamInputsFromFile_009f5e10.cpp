#include "fable_input_manager.h"
// Replay: read every recorded package from the file now, then flag loading.
void CInputManager::StreamInputsFromFile(const CVersion& version, const CWideString& fileName)
{
    CDiskFileWin32 file(fileName, FILE_ACCESS_READ, 2);
    LoadAllEventPackages(version, file);
    Loading = true;
}
