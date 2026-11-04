#pragma once

// CFileInformation (Ego_r.pdb, 0x58): three CDateAndTime stamps and the
// read-only flag. Its default constructor is inline in retail
// (CDiskFileWin32::GetFileInformation 0x0099A5A0 calls the out-of-line
// CDateAndTime() three times and leaves ReadOnly unset).
//
// fable_filesystem.h and several fixtures only forward-declare (or locally
// stand in for) CFileInformation; include this header only where the layout
// is needed.

#include "fable_game.h"

#ifndef _FILETIME_
#define _FILETIME_
// windef.h FILETIME.
typedef struct _FILETIME
{
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME;
#endif

class CFileInformation
{
public:
    CFileInformation() {}

    CDateAndTime CreationTime;      // +0x00
    CDateAndTime LastAccessTime;    // +0x1C
    CDateAndTime LastWriteTime;     // +0x38
    bool ReadOnly;                  // +0x54
};

FABLE_STATIC_ASSERT(sizeof(CFileInformation) == 0x58);
