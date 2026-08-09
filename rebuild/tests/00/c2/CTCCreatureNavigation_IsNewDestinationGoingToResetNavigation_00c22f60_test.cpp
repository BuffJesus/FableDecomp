#include <stdio.h>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Sub {
    CTCCreatureNavigation* got_nav; C3DVector* got_dest; bool ret;
    bool v7(CTCCreatureNavigation* nav, C3DVector* dest){ got_nav=nav; got_dest=dest; return ret; }
};
struct CTCCreatureNavigation {
    void* f0; Sub* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest){ return f4->v7(this, dest); }
};
int main(){
    Sub s; s.ret=true; s.got_nav=0; s.got_dest=0;
    CTCCreatureNavigation nav; nav.f4=&s;
    C3DVector d;
    bool r=nav.IsNewDestinationGoingToResetNavigation(&d);
    if(!(r==true && s.got_nav==&nav && s.got_dest==&d)){ printf("FAIL a\n"); return 1; }
    s.ret=false;
    r=nav.IsNewDestinationGoingToResetNavigation(&d);
    if(!(r==false && s.got_dest==&d)){ printf("FAIL b\n"); return 2; }
    printf("OK_00c22f60\n"); return 0;
}