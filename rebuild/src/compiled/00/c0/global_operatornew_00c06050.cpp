#include "rebuild_abi.h"
__declspec(dllimport) void FABLE_CDECL Imp_c06050(void* p);
extern "C" void FABLE_CDECL Fwd_c06050(void* a, void* b){ Imp_c06050(b); }