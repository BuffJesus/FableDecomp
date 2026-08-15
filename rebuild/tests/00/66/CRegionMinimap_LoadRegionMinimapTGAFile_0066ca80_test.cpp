#include <cstdio>

// Behaviour model of LoadRegionMinimapTGAFile: constructs a temp, builds a request
// keyed on self->field0, then if a check on the wide-string handle passes, finalizes.

struct WS { int valid; };

struct Local { int f0; int rest[7]; };
struct Rgn { int field0; char gap[0x24-4]; int subMarker; };

static int  g_ctor=0, g_build=0, g_finish=0, g_lastBuildKey=-999;

static void ctor(Local* l, int a,int b,int c,int d,int e){ l->rest[0]=a+b+c+d+e; g_ctor++; }
static void build(void* sub, int* key, Local* lo, int z){ g_build++; g_lastBuildKey=*key; (void)sub;(void)lo;(void)z; }
static bool check(WS* w){ return w && w->valid; }
static void finish(void* sub, WS* w, int one){ g_finish++; (void)sub;(void)w;(void)one; }

static void model(Rgn* self, WS* ws){
    Local lo; lo.f0=-1;
    ctor(&lo, 8,8,8,8,1);
    int a=self->field0, b=self->field0; (void)b;
    void* subp=(char*)self+0x24;
    build(subp,&a,&lo,0);
    if(check(ws)) finish(subp,ws,1);
}

int main(){
    Rgn r; r.field0=1234;
    WS good; good.valid=1;
    WS bad; bad.valid=0;

    model(&r,&good);
    bool ok1 = (g_ctor==1 && g_build==1 && g_lastBuildKey==1234 && g_finish==1);

    g_ctor=g_build=g_finish=0; g_lastBuildKey=-999;
    model(&r,&bad);
    bool ok2 = (g_ctor==1 && g_build==1 && g_finish==0);

    if(ok1 && ok2) printf("OK_MINIMAP\n");
    else printf("FAIL c=%d b=%d f=%d k=%d\n",g_ctor,g_build,g_finish,g_lastBuildKey);
    return (ok1&&ok2)?0:1;
}