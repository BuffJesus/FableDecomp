#pragma once

// CBBBFileException: the 4-byte exception CDiskFileWin32 throws (retail RTTI
// .?AVCBBBFileException@@, throw info 0x01369380; Ego_r.pdb sizeof 4, one
// member `Type`). Enumerators are Ego_r.pdb's
// CBBBFileException::EFileExceptionType.
//
// FableWin ??0CBBBFileException@@QAE@VCCharString@@W4EFileExceptionType@0@@Z
// takes the message BY VALUE and is inline in retail: every throw site
// (CDiskFileWin32::SetPosition 0x0099ABF0, MovePosition 0x0099AC90, Read
// 0x0099A960) builds the literal's CCharString temporary, copy-constructs the
// parameter from it, calls operator const char* on the parameter (result
// unused: the release build's message sink compiled away), destroys the
// parameter and only then stores Type.

#include "fable_string.h"
#include "fable_exception.h"

// Retail catchable-type array 0x01369374 also names CExceptionBase (size 1).
// Empty-base optimization preserves Type at +0 and the four-byte object size.
class CBBBFileException : public CExceptionBase
{
public:
    enum EFileExceptionType
    {
        GENERIC = 0,
        BAD_PARAMETERS = 1,
        FILE_NOT_FOUND = 2,
        END_OF_FILE = 3,
        CANNOT_READ = 4,
        CANNOT_WRITE = 5,
        CANNOT_CLOSE = 6,
        CANNOT_OPEN = 7,
        NOT_OPEN_FOR_WRITING = 8,
        NOT_OPEN_FOR_READING = 9,
        BAD_PATH = 10,
        BAD_SEEK = 11,
        BUFFER_TOO_SMALL = 12
    };

    CBBBFileException(CCharString message, EFileExceptionType type)
    {
        static_cast<const char*>(message);
        Type = type;
    }

    // FableWin ??0CBBBFileException@@QAE@W4EFileExceptionType@0@@Z: no
    // message (CMemoryDataOutputStream::SetPosition 0x00994B10).
    explicit CBBBFileException(EFileExceptionType type)
    {
        Type = type;
    }

    EFileExceptionType Type;    // +0x00
};

FABLE_STATIC_ASSERT(sizeof(CBBBFileException) == 0x04);
