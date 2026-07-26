#pragma once

#include "fable_string.h"

class CAFile
{
public:
    static void FABLE_FASTCALL SetCurrentPath(const CWideString& path);
    static CWideString FABLE_FASTCALL GetProjectPath();
};

// CAFile's process-wide working-directory cache. The address suffix is kept
// until the enclosing retail static-data layout has been fully recovered.
extern CWideString g_CAFileCurrentPath_013BCA10;
