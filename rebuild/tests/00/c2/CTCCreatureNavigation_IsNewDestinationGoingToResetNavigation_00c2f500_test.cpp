#include <stdio.h>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
typedef bool (*Fn)(CTCCreatureNavigation*, C3DVector*);
struct Sub { Fn* vtbl; };
struct CTCCreatureNavigation { void* pad0; Sub* sub; };

static CTCCreatureNavigation* g_expSelf; static C3DVector* g_expVec; static bool g_ret;
static bool cb(CTCCreatureNavigation* s, C3DVector* v){ g_expSelf=s; g_expVec=v; return g_ret; }

static bool ModelIsNewDest(CTCCreatureNavigation* self, C3DVector* dest)
{
    Sub* s = self->sub; Fn* vt = s->vtbl; return vt[6](self, dest);
}

int main(){
    Fn vt[8]; for(int i=0;i<8;i++) vt[i]=0; vt[6]=cb;
    Sub s; s.vtbl=vt;
    CTCCreatureNavigation nav; nav.pad0=0; nav.sub=&s;
    C3DVector v; v.x=1; v.y=2; v.z=3;
    g_ret=true;
    bool r = ModelIsNewDest(&nav,&v);
    if(!(r==true && g_expSelf==&nav && g_expVec==&v)){ printf("FAIL1\n"); return 1; }
    g_ret=false;
    r = ModelIsNewDest(&nav,&v);
    if(!(r==false && g_expSelf==&nav && g_expVec==&v)){ printf("FAIL2\n"); return 1; }
    printf("OK_00c2f500\n"); return 0;
}