#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Navigator {
    virtual bool m7(C3DVector* d, CTCCreatureNavigation* n){ called=true; gotd=d; gotn=n; return ret; }
    bool called; C3DVector* gotd; CTCCreatureNavigation* gotn; bool ret;
};
struct CTCCreatureNavigation {
    void* f0; Navigator* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest, float f){ (void)f; return f4->m7(dest,this); }
};
int main(){
    Navigator nav; nav.called=false; nav.ret=true;
    CTCCreatureNavigation obj; obj.f4=&nav; obj.f0=0;
    C3DVector v; v.x=1; v.y=2; v.z=3;
    bool r=obj.IsNewDestinationGoingToResetNavigation(&v, 5.0f);
    if(!nav.called){ printf("FAIL not called\n"); return 1; }
    if(nav.gotd!=&v){ printf("FAIL dest\n"); return 1; }
    if(nav.gotn!=&obj){ printf("FAIL this\n"); return 1; }
    if(r!=true){ printf("FAIL ret\n"); return 1; }
    nav.ret=false;
    r=obj.IsNewDestinationGoingToResetNavigation(&v, 0.0f);
    if(r!=false){ printf("FAIL ret2\n"); return 1; }
    printf("OK_00c09e60\n");
    return 0;
}