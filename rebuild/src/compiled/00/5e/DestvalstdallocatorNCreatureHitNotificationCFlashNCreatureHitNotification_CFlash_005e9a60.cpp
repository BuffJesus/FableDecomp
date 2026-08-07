#include "rebuild_abi.h"
struct C { void Helper(int n); };
extern "C" void FABLE_STDCALL Dest_5e9a60(C* p){ p->Helper(1); }