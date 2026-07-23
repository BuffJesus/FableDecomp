struct CSplineSeg {
    char pad[0x50];
    unsigned char flags;
};

extern float __fastcall CSplineSeg_GetLen(CSplineSeg* s);

struct CSplineRef {
    CSplineSeg* seg;
    int extra;
};

struct CParticleEmitter {
    char pad[8];
    CSplineRef* begin;
    CSplineRef* end;
};

float __fastcall CParticleEmitter_GetApproximateTotalSplineLength(CParticleEmitter* self)
{
    float total = 0.0f;
    CSplineRef* p = self->begin;
    if (p != self->end) {
        do {
            CSplineSeg* seg = p->seg;
            if (seg->flags & 0x2) {
                total += CSplineSeg_GetLen(seg);
            }
            ++p;
        } while (p != self->end);
    }
    return total;
}