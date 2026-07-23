struct DecalObjVtbl {
    long (__fastcall* fns[64])(void*);
};

struct DecalObj {
    DecalObjVtbl* vtbl;
};

struct CParticleSystem {
    char pad0[0xC];
    char* begin;
    char* end;
    char pad14[0x4C - 0x14];
    long base;
};

long __fastcall CParticleSystem_GetNoActiveDecals(CParticleSystem* self)
{
    long total = 0;
    char* p = (char*)self->begin;
    char* endp = (char*)self->end;
    if (p != endp) {
        char* q = p + 8;
        do {
            if (*(char*)(q + 8)) {
                DecalObj* o = *(DecalObj**)q;
                total += o->vtbl->fns[0x6c / 4](o);
            }
            endp = (char*)self->end;
            p += 0x18;
            q += 0x18;
        } while (p != endp);
    }
    return self->base + total;
}