struct Decal { int a; int b; void** obj; int d; char active; char pad[7]; };
struct CParticleSystem { char pad0[0xc]; Decal* begin; Decal* end; char pad1[0x34]; long base; };
typedef long (__fastcall *Fn)(void* self);
long __fastcall CParticleSystem_GetNoActiveDecals(CParticleSystem* self)
{
    long acc = 0;
    Decal* it = self->begin;
    if (it != self->end) {
        void** p = (void**)((char*)it + 8);
        do {
            if (((char*)p)[8]) {
                void** obj = (void**)*p;
                acc += (*(Fn*)((char*)*obj + 0x68))(obj);
            }
            ++it;
            p = (void**)((char*)p + 0x18);
        } while (it != self->end);
    }
    return self->base + acc;
}