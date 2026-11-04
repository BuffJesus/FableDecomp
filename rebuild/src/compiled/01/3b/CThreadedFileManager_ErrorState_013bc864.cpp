#include "fable_gfuninitialise.h"

// Original zero-initialized data, addressed by the read-completion callback.
void (FABLE_FASTCALL* CThreadedFileManager::m_harddiskErrorCallback)(const char*) = 0;
bool CThreadedFileManager::m_bDirtyDiscError = false;
