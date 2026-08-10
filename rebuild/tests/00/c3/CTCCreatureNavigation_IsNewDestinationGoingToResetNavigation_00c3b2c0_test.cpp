#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct ISubNav {
    virtual bool v0(){return false;}
    virtual bool v1(){return false;}
    virtual bool v2(){return false;}
    virtual bool v3(){return false;}
    virtual bool v4(){return false;}
    virtual bool v5(){return false;}
    virtual bool Check(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
    void* f0;
    ISubNav* sub;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
static CTCCreatureNavigation* g_expected_nav=0;
static C3DVector* g_expected_dest=0;
static bool g_ret=false;
struct SubImpl : ISubNav {
    bool Check(CTCCreatureNavigation* nav, C3DVector* dest){
        if(nav!=g_expected_nav||dest!=g_expected_dest){ printf("BADARGS\n"); return false; }
        return g_ret;
    }
};
bool ISubNav::Check(CTCCreatureNavigation* nav, C3DVector* dest){ return false; }
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest){
    return sub->Check(this, dest);
}
int main(){
    SubImpl s;
    CTCCreatureNavigation nav; nav.f0=0; nav.sub=&s;
    C3DVector d={1,2,3};
    g_expected_nav=&nav; g_expected_dest=&d;
    g_ret=true;
    if(nav.IsNewDestinationGoingToResetNavigation(&d)!=true){ printf("FAIL1\n"); return 1; }
    g_ret=false;
    if(nav.IsNewDestinationGoingToResetNavigation(&d)!=false){ printf("FAIL2\n"); return 1; }
    printf("OK_00c3b2c0\n");
    return 0;
}