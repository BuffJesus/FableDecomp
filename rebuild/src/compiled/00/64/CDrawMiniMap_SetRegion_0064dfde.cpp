#pragma optimize("s",on)

struct CRegion;
struct CMiniMapDisplay {
    void SetRegionDisp(CRegion* r);            // 0x82ba00
    void InitialiseOnRegionLoad(void* s);      // 0x82a180
};
struct CDrawMiniMap {
    char pad0[0xc];
    char* f_c;                 // +0x0c
    char pad1[0x28 - 0x10];
    CMiniMapDisplay disp;      // +0x28
    void SetRegion(CRegion* r);
    void ApplyRegion(CRegion* r);              // 0x64dd9a
};
extern unsigned char g_frontendActive;         // ds:0x13b8780
void CDrawMiniMap::SetRegion(CRegion* r) {
    this->disp.SetRegionDisp(r);
    if (g_frontendActive)
        this->disp.InitialiseOnRegionLoad(this->f_c + 0x84);
    this->ApplyRegion(r);
}