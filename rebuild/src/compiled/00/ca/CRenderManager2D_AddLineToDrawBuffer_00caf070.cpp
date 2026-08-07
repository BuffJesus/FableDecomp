#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Helper_caf070(void* self);
struct C { int x; };
extern "C" void FABLE_FASTCALL Handle_caf070(C* self){ if(self){ Helper_caf070((char*)self+4); } else { Helper_caf070(0); } }