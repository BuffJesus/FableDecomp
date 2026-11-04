#include "fable_retail_banks.h"

// The startup and shutdown call sites name the same retail object differently.
// The linker alias gives both names one physical CBankFileManager at 0x013CA79C.
CBankFileManager g_FableBankFileManager_013CA79C;
#pragma comment(linker, "/alternatename:?g_013ca79c@@3VCBankFileManager@@A=?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A")
