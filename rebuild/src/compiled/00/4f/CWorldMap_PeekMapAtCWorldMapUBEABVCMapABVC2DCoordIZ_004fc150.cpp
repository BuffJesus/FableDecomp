// ?GetMapAt@CWorldMap@@UAEAAVCMap@@ABVC2DCoordI@@@Z  0x004fc130
class CEditMap;
class C2DCoordI;

struct MapEntry {   // 72 bytes
    char pad0[0x10];
    CEditMap* map;  // +0x10
    char pad1[72 - 0x10 - 4];
};

struct CWorldMap {
    virtual int vf0();
    virtual int vf1();
    virtual int vf2();
    virtual int vf3();
    virtual int vf4();
    virtual int vf5();
    virtual int vf6();
    virtual int vf7();
    virtual int vf8();
    virtual int vf9();
    virtual int vf10();
    virtual int vf11();
    virtual int vf12();
    virtual int vf13();
    virtual int vf14();
    virtual int vf15();
    virtual int vf16();
    virtual int GetIndex(const C2DCoordI* c);  // slot 17 -> [vtbl+0x44]
    char pad[0x20 - 4];     // vtbl ptr at +0, array ptr at +0x20
    MapEntry* entries;      // +0x20
    CEditMap* GetMapAt(const C2DCoordI* c);
};

CEditMap* CWorldMap::GetMapAt(const C2DCoordI* c)
{
    int i = this->GetIndex(c);
    return this->entries[i].map;
}