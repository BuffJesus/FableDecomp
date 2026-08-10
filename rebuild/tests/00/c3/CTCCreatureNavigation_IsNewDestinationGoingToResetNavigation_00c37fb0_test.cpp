#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Helper {
    virtual void s0(){} virtual void s1(){} virtual void s2(){}
    virtual void s3(){} virtual void s4(){} virtual void s5(){}
    virtual bool s6(CTCCreatureNavigation* nav, C3DVector* v);
};
struct CTCCreatureNavigation {
    void* field0; Helper* field4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
static CTCCreatureNavigation* g_nav; static C3DVector* g_vec; static bool g_ret;
bool Helper::s6(CTCCreatureNavigation* nav, C3DVector* v){
    if(nav!=g_nav){printf("bad nav\n");return false;}
    if(v!=g_vec){printf("bad vec\n");return false;}
    return g_ret;
}
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest){
    return field4->s6(this, dest);
}
int main(){
    Helper h; CTCCreatureNavigation nav; nav.field0=0; nav.field4=&h;
    C3DVector v; v.x=1;v.y=2;v.z=3;
    g_nav=&nav; g_vec=&v;
    g_ret=true;
    if(nav.IsNewDestinationGoingToResetNavigation(&v)!=true){printf("FAIL1\n");return 1;}
    g_ret=false;
    if(nav.IsNewDestinationGoingToResetNavigation(&v)!=false){printf("FAIL2\n");return 1;}
    printf("OK_00c37fb0\n");
    return 0;
}