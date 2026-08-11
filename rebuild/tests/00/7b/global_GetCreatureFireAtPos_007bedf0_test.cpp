#include <cstdio>

struct CThing;

struct CGlob {
    char pad[0x24];
    int m24;
};

CGlob* g_glob;

struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

// local stand-in for the inner helper at 0x7ea2f0
C3DVector __fastcall Inner(int a, CThing* t, float f)
{
    C3DVector v;
    v.x = (float)a;
    v.y = f;
    v.z = t ? 5.0f : -5.0f;
    return v;
}

C3DVector __fastcall GetCreatureFireAtPos(CThing* thing)
{
    return Inner(g_glob->m24, thing, 1.0f);
}

int main()
{
    static CGlob glob_obj;
    glob_obj.m24 = 7;
    CGlob* gp = &glob_obj;
    g_glob = gp;
    CThing* th = (CThing*)0x1000;
    C3DVector r = GetCreatureFireAtPos(th);
    if (r.x == 7.0f && r.y == 1.0f && r.z == 5.0f) {
        printf("FIRE_POS_OK\n");
        return 0;
    }
    printf("FAIL x=%f y=%f z=%f\n", r.x, r.y, r.z);
    return 1;
}