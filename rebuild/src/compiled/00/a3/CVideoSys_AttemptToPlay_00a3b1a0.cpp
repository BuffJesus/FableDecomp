// CVideoSys::AttemptToPlay  0x00a3b1a0
#include "engine/CVideoSys.h"  // retyped onto the PDB layout; byte parity re-verified
struct IMediaControl;
struct IPlayerVtbl {
    // slots 0..6 unknown
    int (__stdcall *slot0)(IMediaControl*);
    int (__stdcall *slot1)(IMediaControl*);
    int (__stdcall *slot2)(IMediaControl*);
    int (__stdcall *slot3)(IMediaControl*);
    int (__stdcall *slot4)(IMediaControl*);
    int (__stdcall *slot5)(IMediaControl*);
    int (__stdcall *slot6)(IMediaControl*);   // +0x18
    int (__stdcall *slot7)(IMediaControl*);   // +0x1c
};
struct IMediaControl {
    IPlayerVtbl* vtbl;
};


bool __fastcall AttemptToPlay(CVideoSys* self)
{
    IMediaControl* p = self->MControl;
    if (p->vtbl->slot7(p) == 0) {
        self->State = 1;
        return true;
    }
    return false;
}