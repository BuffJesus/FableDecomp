#include <stdio.h>

// Behaviour model of CParticleSystem::Kill: calls sub-method at +0x18 with &arg,
// calls method on object at +0x04, then sets bit 4 in flag byte at +0x58.

static int g_sub18_called = 0;
static int g_obj4_called = 0;
static void* g_sub18_argaddr = 0;
static void* g_sub18_self = 0;
static void* g_obj4_self = 0;

struct Obj4 { int tag; };

void Sub18_Method(void* self, void* argAddr){ g_sub18_called=1; g_sub18_self=self; g_sub18_argaddr=argAddr; }
void Obj4_Method(void* self){ g_obj4_called=1; g_obj4_self=self; }

void KillModel(void* self, int arg)
{
    Sub18_Method((char*)self + 0x18, &arg);
    Obj4_Method(*(void**)((char*)self + 4));
    *((unsigned char*)self + 0x58) |= 4;
}

int main()
{
    unsigned char buf[0x60];
    for (int i=0;i<0x60;i++) buf[i]=0;
    Obj4 o; o.tag=7;
    *(void**)(buf+4) = &o;
    buf[0x58] = 1; // pre-set bit0 to check OR keeps it

    KillModel(buf, 12345);

    int ok = 1;
    if (!g_sub18_called) ok=0;
    if (!g_obj4_called) ok=0;
    if (g_sub18_self != (buf+0x18)) ok=0;
    if (g_obj4_self != &o) ok=0;
    if (buf[0x58] != (1|4)) ok=0;

    if (ok) printf("KILL_OK\n");
    else printf("KILL_FAIL sub=%d obj=%d flags=%02x\n", g_sub18_called, g_obj4_called, buf[0x58]);
    return 0;
}