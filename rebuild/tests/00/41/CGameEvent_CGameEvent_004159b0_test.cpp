
#include <stdio.h>
#include <string.h>

struct CGameEventT {
    void* p;
    unsigned char b4;
    char pad[0x25-5];
    unsigned char b25;
    unsigned char b26;
    unsigned char b27;
};

/* function under test: same convention as source (this=ecx via __fastcall, arg on stack) */
CGameEventT* __fastcall CGameEvent_ut(CGameEventT* self, int edx_dummy, void* arg)
{
    self->p = arg;
    self->b4 = 0xff;
    self->b25 = 0;
    self->b26 = 1;
    self->b27 = 0;
    return self;
}

int main()
{
    CGameEventT ev;
    memset(&ev, 0xAA, sizeof(ev));
    int marker = 0x12345678;

    CGameEventT* ret = CGameEvent_ut(&ev, 0, &marker);

    int ok = 1;
    if (ret != &ev) ok = 0;   /* ctor returns this */
    if (ev.p != &marker) ok = 0;
    if (ev.b4 != 0xff) ok = 0;
    if (ev.b25 != 0) ok = 0;
    if (ev.b26 != 1) ok = 0;
    if (ev.b27 != 0) ok = 0;

    if (ok) { printf("OK_0x004159b0\n"); return 0; }
    printf("FAIL p=%p b4=%02x b25=%02x b26=%02x b27=%02x\n",
           ev.p, ev.b4, ev.b25, ev.b26, ev.b27);
    return 1;
}