#include <cstdio>
#include <cmath>
struct C3DVector { float x, y, z; };
struct C2DVector { float x, y; };
struct IPair { int a, b; };

static IPair g_lo, g_hi;
static void MapHelper(IPair* p1, IPair* p2){ *p1=g_lo; *p2=g_hi; }

static void GetRel(C3DVector* a, C2DVector* out){
    IPair lo, hi;
    MapHelper(&lo,&hi);
    out->x=(a->x-(float)lo.a)/(float)hi.a;
    out->y=(a->y-(float)lo.b)/(float)hi.b;
}
int main(){
    g_lo.a=10; g_lo.b=20; g_hi.a=4; g_hi.b=5;
    C3DVector a={30.0f,45.0f,0.0f};
    C2DVector o;
    GetRel(&a,&o);
    // x=(30-10)/4=5 ; y=(45-20)/5=5
    if(fabs(o.x-5.0f)<1e-4 && fabs(o.y-5.0f)<1e-4) printf("BEHAVIOR_PASS\n");
    else printf("FAIL %f %f\n",o.x,o.y);
    return 0;
}