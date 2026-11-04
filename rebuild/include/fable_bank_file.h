#pragma once

#include "fable_string.h"

namespace NBankFileManager
{
    void FABLE_FASTCALL SetHeaderDir(
        const CWideString& headerDirectory);
    // FableWin ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ, retail
    // 0x009A76A0: a copy of the directory SetHeaderDir 0x009A76C0 stores
    // (retail 0x013CA7D4).
    CWideString FABLE_FASTCALL GetHeaderDir();
}
