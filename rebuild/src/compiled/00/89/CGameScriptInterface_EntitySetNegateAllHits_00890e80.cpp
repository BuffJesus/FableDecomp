struct Entity {
    unsigned char _p0[0x6c];
    unsigned char f6c;
    unsigned char _p6d[0x24];
    unsigned char f91;
    unsigned char _p92[0x2a];
    unsigned char fbc;
};

struct GSIvt {
    unsigned char _s0[0x2c];
    Entity* (__fastcall *getEntity)(void* self);
};
struct GSI { GSIvt* vt; };

void __stdcall CGameScriptInterface_EntitySetNegateAllHits(GSI* self, unsigned char value)
{
    Entity* e = self->vt->getEntity(self);
    if (!e) return;
    if (e->f91 & 1) return;
    if (!(e->f6c & 2)) return;
    unsigned char cl = (unsigned char)(value << 4);
    cl ^= e->fbc;
    cl &= 0x10;
    e->fbc ^= cl;
}