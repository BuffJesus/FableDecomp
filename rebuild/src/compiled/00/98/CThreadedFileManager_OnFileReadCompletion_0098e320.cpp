// Retail 0098E320; private static stdcall identity confirmed by FableWin PDB.
#include "fable_threaded_file_object.h"
#include "fable_threaded_file_overlapped.h"
#include "fable_file_installer.h"
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);

void __stdcall CThreadedFileManager::OnFileReadCompletion(unsigned long error, unsigned long, _OVERLAPPED* overlapped)
{
    CFileObject* object = static_cast<CFileObject*>(overlapped->hEvent);
    if (error) {
        if (object->PFile->deviceId_ == THREADED_FILE_DEVICE_ID_DVD)
            m_bDirtyDiscError = true;
        else if (m_harddiskErrorCallback)
            m_harddiskErrorCallback("");
    }
    object->LoadingStatus = FILE_LOADING_STATUS_READ_COMPLETED;
    SetEvent(object->PManager->Event);
    if (g_CFileInstallerSingleton_013CA818.object)
        CFileInstallerSingleton::Get().Unpause();
}
