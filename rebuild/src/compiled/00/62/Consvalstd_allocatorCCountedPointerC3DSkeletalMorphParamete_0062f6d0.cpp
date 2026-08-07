#include "rebuild_abi.h"
// _Cons_val CCountedPointer copy @ 0x0062f6d0
// if(!ecx)ret; [ecx]=[edx]; eax=[edx+4]; [ecx+4]=eax; if(eax) ++*eax;
struct CCP { void *m_a; int *m_ref; };
extern "C" void FABLE_FASTCALL FableCons_62f6d0(CCP *dst, const CCP *src){
    if (!dst) return;
    dst->m_a = src->m_a;
    int *r = src->m_ref;
    dst->m_ref = r;
    if (r) ++*r;
}