struct C3DVector { float x, y, z; };
struct C2DVector { float x, y; };
struct IPair { int a, b; };

// callee at 0x66c6a0 is a real __fastcall member on the (this+0x54) subobject.
struct MapSub {
    void GetBounds(IPair* p1, IPair* p2);
};

struct CMiniMapDisplay {
    char pad[0x54];
    MapSub sub;
};

void __fastcall GetRelativePosOnMap(CMiniMapDisplay* self, void* edx,
                                    C3DVector* a, C2DVector* out)
{
    IPair lo;
    IPair hi;
    self->sub.GetBounds(&lo, &hi);
    out->x = (a->x - (float)lo.a) / (float)hi.a;
    out->y = (a->y - (float)lo.b) / (float)hi.b;
}