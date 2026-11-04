// NBankFileManager::GetHeaderDir, retail 0x009A76A0 (FableWin
// ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ): a copy of the bank
// header directory SetHeaderDir 0x009A76C0 stores at 0x013CA7D4.
#include "fable_bank_file.h"

extern "C" fable_u8 g_FableBankHeaderDirectory_013CA7D4[
    sizeof(CWideString)];

CWideString FABLE_FASTCALL NBankFileManager::GetHeaderDir()
{
    return reinterpret_cast<const CWideString&>(
        g_FableBankHeaderDirectory_013CA7D4);
}
