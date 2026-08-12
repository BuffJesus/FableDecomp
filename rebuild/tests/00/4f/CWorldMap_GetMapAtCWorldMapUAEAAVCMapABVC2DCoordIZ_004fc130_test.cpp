#include <cstdio>

class CEditMap;
class C2DCoordI;

struct MapEntry {
    char pad0[0x10];
    CEditMap* map;
    char pad1[72 - 0x10 - 4];
};

static int g_wantIndex = 0;

struct CWorldMap {
    virtual int vf0() { return 0; }
    virtual int vf1() { return 0; }
    virtual int vf2() { return 0; }
    virtual int vf3() { return 0; }
    virtual int vf4() { return 0; }
    virtual int vf5() { return 0; }
    virtual int vf6() { return 0; }
    virtual int vf7() { return 0; }
    virtual int vf8() { return 0; }
    virtual int vf9() { return 0; }
    virtual int vf10() { return 0; }
    virtual int vf11() { return 0; }
    virtual int vf12() { return 0; }
    virtual int vf13() { return 0; }
    virtual int vf14() { return 0; }
    virtual int vf15() { return 0; }
    virtual int vf16() { return 0; }
    virtual int GetIndex(const C2DCoordI* c) { return g_wantIndex; }
    char pad[0x20 - 4];
    MapEntry* entries;
    CEditMap* GetMapAt(const C2DCoordI* c);
};

CEditMap* CWorldMap::GetMapAt(const C2DCoordI* c)
{
    int i = this->GetIndex(c);
    return this->entries[i].map;
}

int main()
{
    MapEntry arr[4];
    for (int k = 0; k < 4; ++k)
        arr[k].map = (CEditMap*)(0x1000 + k * 0x10);

    CWorldMap m;
    m.entries = arr;

    g_wantIndex = 2;
    CEditMap* r = m.GetMapAt((const C2DCoordI*)0);
    if (r != (CEditMap*)(0x1000 + 2 * 0x10)) { printf("FAIL idx2\n"); return 1; }

    g_wantIndex = 0;
    r = m.GetMapAt((const C2DCoordI*)0);
    if (r != (CEditMap*)0x1000) { printf("FAIL idx0\n"); return 1; }

    printf("GETMAPAT_OK\n");
    return 0;
}