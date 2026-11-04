#include "fable_filesystem.h"

void (FABLE_FASTCALL* CAFile::m_errorCallback)() = 0;
void (FABLE_FASTCALL* CAFile::m_harddiskErrorCallback)(const char*) = 0;
