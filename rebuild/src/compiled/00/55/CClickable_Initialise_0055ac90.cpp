#include "rebuild_abi.h"

// CClickable::Initialise @ 0x0055ac90
// push esi; mov esi,ecx; call base_init; mov eax,[esi+0x30];
// mov [esi+0x190],eax; pop esi; ret
struct FableClickable_0055ac90
{
    char pad30[0x30];
    unsigned long field_30;      // +0x30
    char pad[0x190 - 0x34];
    unsigned long field_190;     // +0x190
    void FABLE_FASTCALL base_init();
};

extern "C" void FABLE_FASTCALL FableClickableInit_0055ac90(FableClickable_0055ac90* self)
{
    self->base_init();
    self->field_190 = self->field_30;
}