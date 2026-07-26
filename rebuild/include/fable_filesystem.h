#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString
{
public:
    CWideString();
    explicit CWideString(const wchar_t* text);
    operator const wchar_t*() const;
    CWideString& operator=(const CWideString& other);

private:
    fable_u32 storage_;
};

class CAFile
{
public:
    static void FABLE_FASTCALL SetCurrentPath(const CWideString& path);
    static CWideString FABLE_FASTCALL GetProjectPath();
};

// CAFile's process-wide working-directory cache. The address suffix is kept
// until the enclosing retail static-data layout has been fully recovered.
extern CWideString g_CAFileCurrentPath_013BCA10;

FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
