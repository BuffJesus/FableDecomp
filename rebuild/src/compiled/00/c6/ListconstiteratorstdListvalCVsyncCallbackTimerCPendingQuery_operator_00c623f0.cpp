#include "rebuild_abi.h"
struct N { int f0; int* f4; };
extern "C" int FABLE_STDCALL FableRet_c623f0(N** p){
    N* a = *p;
    int* d = a->f4;
    return *d;
}