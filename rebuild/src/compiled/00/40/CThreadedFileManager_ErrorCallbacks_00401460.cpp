// Retail callback policy leaves; original x86 fastcall and PDB access preserved.
#include "fable_gfuninitialise.h"

CThreadedFileManager::ErrorCallback CThreadedFileManager::m_errorCallback = 0;

void FABLE_FASTCALL CThreadedFileManager::SetErrorCallback(ErrorCallback callback)
{
    m_errorCallback = callback;
}

void FABLE_FASTCALL CThreadedFileManager::SetHardDiskErrorCallback(HardDiskErrorCallback callback)
{
    m_harddiskErrorCallback = callback;
}

void FABLE_FASTCALL CThreadedFileManager::SetDirtyDiscError(bool dirty)
{
    m_bDirtyDiscError = dirty;
}

CThreadedFileManager::HardDiskErrorCallback FABLE_FASTCALL CThreadedFileManager::GetHardDiskErrorCallback()
{
    return m_harddiskErrorCallback;
}

void FABLE_FASTCALL CThreadedFileManager::CallHardDiskErrorCallback(const char* text)
{
    m_harddiskErrorCallback(text);
}

void FABLE_FASTCALL CThreadedFileManager::DoHardDiskErrorCallback(const char* text)
{
    if (m_harddiskErrorCallback)
        m_harddiskErrorCallback(text);
}
