#pragma once

#include <stddef.h>

#include "fable_string.h"

class CDefStringTableRecovered
{
public:
    void SetTablePath(const CWideString& pathname);

    fable_u8 unknown00_[0x30];
    CWideString tablePath_;
};

FABLE_STATIC_ASSERT(
    offsetof(CDefStringTableRecovered, tablePath_) == 0x30);

class CDefStringTable
{
public:
    void LoadTable(bool readOnly);

    fable_u8 unknown00_[0x30];
    CWideString tablePath_;
    bool loaded_;
    bool readOnly_;
};

FABLE_STATIC_ASSERT(offsetof(CDefStringTable, tablePath_) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, loaded_) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, readOnly_) == 0x35);
