#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString
{
public:
    CWideString();
    explicit CWideString(const wchar_t* text);
    ~CWideString();
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

// Retail folds equivalent narrow/wide string lifetime helpers. Keep one
// neutral name for the shared live-object counter until that ownership unit
// is promoted as a whole.
extern fable_i32 g_CStringInstanceCount_013BCA20;

FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
