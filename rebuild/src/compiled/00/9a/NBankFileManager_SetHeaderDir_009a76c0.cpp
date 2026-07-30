#include "fable_bank_file.h"

extern "C" fable_u8 g_FableBankHeaderDirectory_013CA7D4[
    sizeof(CWideString)];

void FABLE_FASTCALL NBankFileManager::SetHeaderDir(
    const CWideString& headerDirectory)
{
    reinterpret_cast<CWideString&>(
        g_FableBankHeaderDirectory_013CA7D4) =
            headerDirectory;
}
