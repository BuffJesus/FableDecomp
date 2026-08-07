#include "rebuild_abi.h"
struct C { void Helper(void* b); };
extern "C" void FABLE_STDCALL New_c35b10(C* a, void* b){ a->Helper(b); }