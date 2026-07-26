#include <cstdio>

struct CSplineSeg {
    char pad[0x50];
    unsigned char flags;
    float len;
};

float __fastcall CSplineSeg_GetLen(CSplineSeg* s) { return s->len; }

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

int main() {
    CSplineSeg a, b, c;
    a.flags = 0x2; a.len = 1.5f;
    b.flags = 0x0; b.len = 100.0f;
    c.flags = 0x3; c.len = 2.5f;
    CSplineRef arr[3];
    arr[0].seg = &a; arr[1].seg = &b; arr[2].seg = &c;
    CParticleEmitter e;
    e.begin = arr;
    e.end = arr + 3;
    float r = CParticleEmitter_GetApproximateTotalSplineLength(&e);
    if (r == 4.0f) { std::printf("CParticleEmitter_00aed050_TEST PASS\n"); return 0; }
    std::printf("FAIL got %f\n", r); return 1;
}