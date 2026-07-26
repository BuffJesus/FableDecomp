struct Ring { int dummy; };
extern void __fastcall Ring_dtor(Ring* r);
extern void __cdecl op_free(void* p);
struct RingVec { Ring* first; Ring* last; };

void __fastcall CCombatWheel_ResetRings(RingVec* self)
{
    Ring* p = self->first;
    Ring* e = self->last;
    while (p != e) {
        Ring_dtor(p);
        p++;
    }
    if (self->first)
        op_free(self->first);
}