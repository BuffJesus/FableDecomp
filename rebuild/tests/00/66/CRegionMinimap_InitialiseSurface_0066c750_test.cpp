#include <cstdio>
struct C2DExtentsI { int x; int y; };

static int g_calls=0, g_r,g_g,g_b,g_a,g_signd,g_extx,g_exty;

struct CPixelFormat {
    int a;
    void Initialise(long r, long g, long b2, long a2, bool signd){
        g_r=r; g_g=g; g_b=b2; g_a=a2; g_signd=signd; g_calls++;
    }
};
struct CSurface {
    char pad[8];
    bool InitialiseAsImage(C2DExtentsI* ext, CPixelFormat* pf, bool bb){
        (void)pf;(void)bb; g_extx=ext->x; g_exty=ext->y; g_calls++; return true;
    }
};
struct CRegionMinimap {
    int handle;
    char pad[0x20];
    CSurface surf;
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

int main(){
    CRegionMinimap rm; rm.handle = 77;
    InitialiseSurface(&rm);
    if(g_calls==2 && g_r==8 && g_g==8 && g_b==8 && g_a==0 && g_signd==1 && g_extx==77 && g_exty==77)
        printf("SURFACE_OK\n");
    else printf("FAIL c=%d r=%d g=%d b=%d a=%d s=%d x=%d y=%d\n",g_calls,g_r,g_g,g_b,g_a,g_signd,g_extx,g_exty);
    return 0;
}