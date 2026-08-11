struct CThing;

struct CGlob {
    char pad[0x24];
    int m24;
};

extern CGlob* g_glob;

struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

C3DVector __fastcall Inner(int a, CThing* t, float f);

C3DVector __fastcall GetCreatureFireAtPos(CThing* thing)
{
    return Inner(g_glob->m24, thing, 1.0f);
}