// CASuspendableProcess::TerminateProcess @ 0x00a4b200
// Sets the terminate-requested flag, then pumps a virtual until the process
// is no longer active.

struct CASuspendableProcess;

struct CASuspendableProcessVtbl {
    void (__fastcall *slot0)(CASuspendableProcess* self);
    void (__fastcall *Pump)(CASuspendableProcess* self);   // vtable slot 1 (+4)
};

#pragma pack(push,1)
struct CASuspendableProcess {
    CASuspendableProcessVtbl* vtbl;   // +0 (4 bytes)
    unsigned char active;             // +4
    unsigned char terminateRequested; // +5
};
#pragma pack(pop)

void __fastcall TerminateProcess(CASuspendableProcess* self)
{
    self->terminateRequested = 1;
    while (self->active) {
        self->vtbl->Pump(self);
    }
}