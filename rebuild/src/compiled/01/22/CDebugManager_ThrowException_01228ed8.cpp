#include "rebuild_abi.h"
// CDebugManager::ThrowException @ 0x01228ed8
extern "C" void FABLE_STDCALL FableThrow_1228ed8(void *a, int b, int c, void *d);
void FABLE_CDECL FableThrowException_1228ed8(){
    FableThrow_1228ed8((void*)0x13b8b10, 4, 0xa, (void*)0x99eae0);
}