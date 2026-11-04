#pragma once

// The inline body of CDataOutputStream::WriteULONG for the retail units that
// expand it (CDefStringTable::AddString 0x009D4F90). Most units call the
// out-of-line copy (0x00993EA0) instead, so fable_data_output_stream.h only
// declares it.

#include "fable_data_output_stream.h"

inline void CDataOutputStream::WriteULONG(unsigned long value)
{
    Write(&value, 4);
}
