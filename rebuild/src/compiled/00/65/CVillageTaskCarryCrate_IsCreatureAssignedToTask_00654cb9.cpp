#include "rebuild_abi.h"
extern "C" void* FABLE_FASTCALL Task_94b330_A(void* arg);
extern "C" bool FABLE_FASTCALL Is_94b330(void* self, void* arg){ void* r=Task_94b330_A(arg); return self==r; }