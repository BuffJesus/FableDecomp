struct Ring { int x; };
extern "C" void __fastcall Ring_dtor(Ring* r);
extern "C" void engine_free(void* p);

struct CCombatWheel {
    Ring* begin;
    Ring* end;
};

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* p = self->begin;
    Ring* e = self->end;
    if (p != e) {
        do {
            Ring_dtor(p);
            p++;
        } while (p != e);
    }
    Ring* b = self->begin;
    if (b) {
        engine_free(b);
    }
}