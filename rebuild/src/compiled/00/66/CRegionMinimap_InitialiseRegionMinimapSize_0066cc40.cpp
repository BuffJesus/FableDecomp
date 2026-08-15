// CRegionMinimap::InitialiseRegionMinimapSize  0x0066cc40  __fastcall void(CWorldMap*, CRegion*)
struct CRegion;

struct RegionVec {
    CRegion** begin;
    CRegion** end;
    CRegion** cap;                 // third field -> 16-byte local frame
    void Build(CRegion* region);   // 0x514160 __fastcall(CRegion*)
};
extern "C" void __cdecl FreeArrayBuf(void* p);

// CWorldMap: model the two virtual slots at +0x28 and +0x4c.
// 0x28/4 = slot 10, 0x4c/4 = slot 19.
struct CWorldMap {
    virtual void  s0();  virtual void s1();  virtual void s2();  virtual void s3();
    virtual void  s4();  virtual void s5();  virtual void s6();  virtual void s7();
    virtual void  s8();  virtual void s9();
    virtual char  TestRegion(CRegion* r);      // slot 10  (+0x28)
    virtual void  s11(); virtual void s12(); virtual void s13(); virtual void s14();
    virtual void  s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual int   GetRegionExtent(CRegion* r); // slot 19  (+0x4c)
};

struct CRegionMinimap {
    void SetRegionMapExtents(int val);              // 0x66c630 __fastcall(int)
    void InitialiseRegionMinimapSize(CWorldMap* worldMap, CRegion* region);
};

void CRegionMinimap::InitialiseRegionMinimapSize(CWorldMap* worldMap, CRegion* region)
{
    RegionVec vec;
    vec.Build(region);
    CRegion** begin = vec.begin;
    int count = (int)(vec.end - begin);
    for (int i = 0; i < count; ++i) {
        CRegion* r = begin[i];
        if (worldMap->TestRegion(r)) {
            int v = worldMap->GetRegionExtent(r);
            this->SetRegionMapExtents(v);
        }
        begin = vec.begin; // reload
    }
    if (vec.begin) FreeArrayBuf(vec.begin);
}