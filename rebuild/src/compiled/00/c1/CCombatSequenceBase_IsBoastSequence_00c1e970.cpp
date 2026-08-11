// CCombatSequenceBase::IsBoastSequence  @ 0x00c1e970
// Retail:
//   mov eax,[ecx]      ; vtable
//   push esi; mov esi,edx
//   call [eax+8]       ; virtual slot 2 -> ecx
//   mov ecx,eax; xor edx,edx; mov eax,esi; div ecx
//   mov edx,eax; imul edx,ecx; mov eax,esi; sub eax,edx  ; eax = esi % ecx
//   pop esi; ret
// i.e. returns (unsigned)param % this->vtbl[2]()  as bool.

struct CCombatSequenceBase
{
    virtual unsigned int v0();
    virtual unsigned int v1();
    virtual unsigned int GetPeriod();   // slot 2 -> call [eax+8]
};

// ecx = self, edx = value (a second register-passed unsigned param).
// Retail returns the raw modulo remainder in eax (no bool normalization),
// so the return type is the integer remainder; caller treats low byte as bool.
unsigned int __fastcall CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* self, unsigned int value)
{
    unsigned int period = self->GetPeriod();
    return value - (value / period) * period;
}