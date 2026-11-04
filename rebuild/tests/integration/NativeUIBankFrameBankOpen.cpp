// Retail-bank archive open/close in its own unit: fable_retail_banks.h and the system-manager headers both
// declare CBankFileManager.
#include "fable_retail_banks.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CWideString g_CAFileCurrentPath_013BCA10;
extern "C" void OpenFontArchive(const char* file){CCharString filename(file);CWideString path=filename.ToWideString();CBankFileManager& banks=g_FableBankFileManager_013CA79C;banks.UseRetailBanks=true;banks.OpenRetailBank(path,false);}
extern "C" void CloseFontArchive(){g_FableBankFileManager_013CA79C.Close();g_FableBankFileManager_013CA79C.Close();g_CAFileCurrentPath_013BCA10=L"";}
