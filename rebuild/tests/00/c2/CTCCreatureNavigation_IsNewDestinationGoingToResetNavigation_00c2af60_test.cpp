
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Sub {
    bool result;
    void* self_seen;
    void* v_seen;
    bool Query(CTCCreatureNavigation* outer, C3DVector* v){ self_seen=outer; v_seen=v; return result; }
};
struct CTCCreatureNavigation {
    void* m_x0;
    Sub* m_sub;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest){ return m_sub->Query(this, dest); }
};
int main(){
    Sub s; s.result=true; s.self_seen=0; s.v_seen=0;
    CTCCreatureNavigation nav; nav.m_x0=0; nav.m_sub=&s;
    C3DVector d = {1,2,3};
    bool r = nav.IsNewDestinationGoingToResetNavigation(&d);
    bool ok = r==true && s.self_seen==(void*)&nav && s.v_seen==(void*)&d;
    s.result=false;
    bool r2 = nav.IsNewDestinationGoingToResetNavigation(&d);
    ok = ok && r2==false;
    if(ok){ printf("OK_00c2af60\n"); return 0; }
    printf("FAIL\n"); return 1;
}