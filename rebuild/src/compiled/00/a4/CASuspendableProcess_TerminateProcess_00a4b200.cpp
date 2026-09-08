#include "engine/CASuspendableProcess.h"  // retyped onto the PDB layout; byte parity re-verified

struct CASuspendableProcessVTable {
    void (__fastcall *Slot0)(CASuspendableProcess* self);
    void (__fastcall *Pump)(CASuspendableProcess* self);
};

void __fastcall TerminateProcess(CASuspendableProcess* self)
{
    self->StopRunning = true;
    while (self->Running) {
        CASuspendableProcessVTable* vtable =
            (CASuspendableProcessVTable*)self->__vftable;
        vtable->Pump(self);
    }
}
