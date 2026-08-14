#include <cstdio>

struct IPlayer;
struct IPlayerVtbl {
    int (__stdcall *slot0)(IPlayer*);
    int (__stdcall *slot1)(IPlayer*);
    int (__stdcall *slot2)(IPlayer*);
    int (__stdcall *slot3)(IPlayer*);
    int (__stdcall *slot4)(IPlayer*);
    int (__stdcall *slot5)(IPlayer*);
    int (__stdcall *slot6)(IPlayer*);
    int (__stdcall *slot7)(IPlayer*);
};
struct IPlayer { IPlayerVtbl* vtbl; };
struct CVideoSys {
    void* m0;
    IPlayer* m4;
    char pad[0x28-0x08];
    int  m28;
};

bool __fastcall AttemptToPlay(CVideoSys* self)
{
    IPlayer* p = self->m4;
    if (p->vtbl->slot7(p) == 0) {
        self->m28 = 1;
        return true;
    }
    return false;
}

static int g_ret;
static int __stdcall busy(IPlayer*) { return g_ret; }

int main()
{
    IPlayerVtbl vt; vt.slot7 = busy;
    IPlayer pl; pl.vtbl = &vt;
    CVideoSys vs; vs.m4 = &pl;

    g_ret = 0; vs.m28 = 0;
    bool r1 = AttemptToPlay(&vs);
    if (!(r1 && vs.m28 == 1)) { printf("FAIL1\n"); return 1; }

    g_ret = 5; vs.m28 = 99;
    bool r2 = AttemptToPlay(&vs);
    if (!(!r2 && vs.m28 == 99)) { printf("FAIL2\n"); return 1; }

    printf("VERIFY_OK\n");
    return 0;
}