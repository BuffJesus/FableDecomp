#include "rebuild_abi.h"
// _Cons_val ... @ 0x00c19680
// eax=[ecx+0x4]; ecx=[esp+4]; eax+=0x3c; if(ecx) eax=[ecx+0xc]; ret 4
struct ConsThis { char pad[4]; char *m_p; void *Cons(void *arg); };
void *ConsThis::Cons(void *arg)
{
    char *r = m_p + 0x3c;
    if (arg)
        r = (char*)arg + 0xc;
    return r;
}