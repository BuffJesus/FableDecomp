#include "fable_input_manager.h"
// Start recording: the package file opens for creation with the version first.
void CInputManager::StreamInputsToFile(const CVersion& version, const CWideString& fileName)
{
    OpenPackageFileForSaving(version, fileName);
    Saving = true;
}
