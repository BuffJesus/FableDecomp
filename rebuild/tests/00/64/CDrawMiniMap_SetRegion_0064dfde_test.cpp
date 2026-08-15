
#include <cstdio>
struct CRegion { int id; };
struct CMiniMapDisplay { int lastRegion; void* lastInit; int initCount; };
static void dispSet(CMiniMapDisplay* d, CRegion* r){ d->lastRegion = r? r->id : -1; }
static void dispInit(CMiniMapDisplay* d, void* s){ d->lastInit = s; d->initCount++; }
struct CDrawMiniMap { char pad0[0xc]; char* f_c; char pad1[0x28-0x10]; CMiniMapDisplay disp; };
static int g_active=0, g_applyCount=0; static CRegion* g_applied=0;
static void applyRegion(CDrawMiniMap* m, CRegion* r){ g_applyCount++; g_applied=r; }
static void setRegion(CDrawMiniMap* m, CRegion* r){
    dispSet(&m->disp, r);
    if (g_active) dispInit(&m->disp, m->f_c + 0x84);
    applyRegion(m, r);
}
int main(){
    CDrawMiniMap m; for(int i=0;i<(int)sizeof(m);++i) ((char*)&m)[i]=0;
    static char base[0x200]; m.f_c = base;
    CRegion r; r.id=42;
    g_active=0; g_applyCount=0; m.disp.initCount=0;
    setRegion(&m,&r);
    if(m.disp.lastRegion!=42 || m.disp.initCount!=0 || g_applyCount!=1 || g_applied!=&r){printf("F1\n");return 1;}
    g_active=1; g_applyCount=0; m.disp.initCount=0;
    setRegion(&m,&r);
    if(m.disp.initCount!=1 || m.disp.lastInit!=(base+0x84) || g_applyCount!=1){printf("F2\n");return 1;}
    printf("SETREGION_OK\n"); return 0;
}