// UI bank-frame fixture data and cold boundaries (NativeUIBankFrame_test.cpp).
// Data: globals whose defining sources belong to other lanes (text-entry read context, CAFile debug flag).
// Cold: the bank file's disk/parse/date/CRC paths (symbol-map file loading, checksum caching,
// modification times) are unreachable for a read-only retail-bank font bank and fail fast.
#include "fable_filesystem.h"
unsigned long g_NativeTextReadContext_0138DE48=0x12345678,g_NativeTextReadContext_0138E064=0x87654321;
bool CAFile::DebugUseReadOnlyDrives=false;
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CDateAndTime@@QAE@JKKKKKK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CDateAndTime@@QAE@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Initialise@CDateAndTime@@QAEXABU_FILETIME@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Calc@CCRC@@QBEIPBXI@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z=_ColdBoundary")
// Error-message formatting on bank format/version failures (cold here); their units redefine CCharString
// constructors inline, which collides with the strong landings under /Gy.
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@PBDABV0@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GFIntToCharString@@YI?AVCCharString@@J@Z=_ColdBoundary")
// Sub-string extraction (filename/path parsing on disk paths, cold for a retail-bank font bank); same inline
// constructor collision as above.
#pragma comment(linker,"/alternatename:?GetSubString@CCharString@@QBE?AV1@KK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetSubString@CWideString@@QBE?AV1@KK@Z=_ColdBoundary")
