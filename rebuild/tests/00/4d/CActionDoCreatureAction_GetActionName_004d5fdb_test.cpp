#include <cstdio>
struct CCharString { const char* p; int n; };
static const char* g_last_s; static int g_last_n; static CCharString* g_last_self;
CCharString* ccs_ctor(CCharString* self, const char* s, int n){ g_last_self=self; g_last_s=s; g_last_n=n; self->p=s; self->n=n; return self; }
// Behavioural model of GetActionName_7: retbuf in, construct CCharString(0x123dd14,-1), return retbuf.
CCharString* getname(CCharString* retbuf){ ccs_ctor(retbuf, (const char*)0x123dd14, -1); return retbuf; }
int main(){
    CCharString buf; buf.p=0; buf.n=0;
    CCharString* r = getname(&buf);
    int ok = (r==&buf) && (g_last_self==&buf) && (g_last_s==(const char*)0x123dd14) && (g_last_n==-1) && (buf.p==(const char*)0x123dd14) && (buf.n==-1);
    // exercise the failure side of the model too
    CCharString buf2; buf2.p=0; buf2.n=0; ccs_ctor(&buf2, (const char*)0x1, 0);
    int ok2 = (buf2.p==(const char*)0x1) && (buf2.n==0);
    printf((ok&&ok2)?"OK_004d5fdb\n":"FAIL\n");
    return (ok&&ok2)?0:1;
}