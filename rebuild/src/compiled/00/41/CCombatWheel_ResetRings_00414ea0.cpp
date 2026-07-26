struct Ring { int v; void Reset(); };
struct CCombatWheel {
    Ring* begin;
    Ring* end_;
};
extern void __cdecl ext_free(void*);

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* p = self->begin;
    Ring* e = self->end_;
    while (p != e) {
        p->Reset();
        ++p;
    }
    Ring* base = self->begin;
    if (base) ext_free(base);
}