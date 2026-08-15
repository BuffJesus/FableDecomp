// CRegionMinimap::InitialiseSurface  0x0066c750  __fastcall void(CRegionMinimap*)
struct C2DExtentsI { int x; int y; };

struct CPixelFormat {
    int a;
    void Initialise(long r, long g, long b2, long a2, bool signd);
};
struct CSurface {
    char pad[8];
    bool InitialiseAsImage(C2DExtentsI* ext, CPixelFormat* pf, bool bb);
};
struct CRegionMinimap {
    int handle;
    char pad[0x20];
    CSurface surf;   // +0x24
};

void __fastcall InitialiseSurface(CRegionMinimap* self)
{
    CPixelFormat pf;
    int dim = self->handle;
    *(int*)&pf = -1;
    pf.Initialise(8, 8, 8, 0, true);
    C2DExtentsI ext;
    ext.x = dim;
    ext.y = dim;
    self->surf.InitialiseAsImage(&ext, &pf, false);
}