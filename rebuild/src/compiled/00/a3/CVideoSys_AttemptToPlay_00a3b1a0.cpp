// CVideoSys::AttemptToPlay  0x00a3b1a0
struct IPlayer;
struct IPlayerVtbl {
    // slots 0..6 unknown
    int (__stdcall *slot0)(IPlayer*);
    int (__stdcall *slot1)(IPlayer*);
    int (__stdcall *slot2)(IPlayer*);
    int (__stdcall *slot3)(IPlayer*);
    int (__stdcall *slot4)(IPlayer*);
    int (__stdcall *slot5)(IPlayer*);
    int (__stdcall *slot6)(IPlayer*);   // +0x18
    int (__stdcall *slot7)(IPlayer*);   // +0x1c
};
struct IPlayer {
    IPlayerVtbl* vtbl;
};

struct CVideoSys {
    void* m0;         // +0x00
    IPlayer* m4;      // +0x04
    char pad[0x28-0x08];
    int  m28;         // +0x28
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