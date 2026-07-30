#pragma once

#include "fable_string.h"

class CBankFileRecovered
{
public:
    CWideString GetBankHandle() const;

    // Recovered layout view; only the handle offset is currently named.
    fable_u8 unknown00_[0x28];
    CWideString bankHandle_;
};

FABLE_STATIC_ASSERT(offsetof(CBankFileRecovered, bankHandle_) == 0x28);

namespace NBankFileManager
{
    void FABLE_FASTCALL SetHeaderDir(
        const CWideString& headerDirectory);
}
