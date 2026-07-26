// CRegion::SetRegionDefName @ 006bc1d0
struct NameField {
    void Assign(int name);   // thiscall, 1 arg  -> 0x2e2de0
};

struct Counted {
    int GetCount();          // thiscall, 0 arg  -> 0x2f1240
};

struct Applied {
    void Apply();            // thiscall, 0 arg  -> 0xe90
};

extern "C" Counted* __stdcall Resolve(NameField* p);     // 0xffd904e0
extern "C" Applied* __stdcall Resolve2(int count, void* p); // 0xffd904e0

struct CRegion {
    char pad0[0x20];
    NameField field20; // at +0x20
    int field24;       // at +0x24
};

void __fastcall CRegion_SetRegionDefName(CRegion* self, int /*edx*/, int name)
{
    self->field20.Assign(name);
    Counted* c = Resolve(&self->field20);
    int cnt = c->GetCount();
    if (cnt > 0) {
        Applied* a = Resolve2(cnt, &self->field24);
        a->Apply();
    }
}