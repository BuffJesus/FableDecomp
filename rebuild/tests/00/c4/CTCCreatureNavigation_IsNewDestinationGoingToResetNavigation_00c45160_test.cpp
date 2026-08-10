#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Helper {
    virtual bool m6(CTCCreatureNavigation* nav, C3DVector* v);
    int called; C3DVector* seen; CTCCreatureNavigation* seennav; bool ret;
};
struct CTCCreatureNavigation {
    void* f0; Helper* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v) {
        return f4->m6(this, v);
    }
};
bool Helper::m6(CTCCreatureNavigation* nav, C3DVector* v){ called=1; seennav=nav; seen=v; return ret; }
int main(){
    Helper h; h.called=0; h.ret=true;
    CTCCreatureNavigation nav; nav.f4=&h;
    C3DVector vec={1,2,3};
    bool r = nav.IsNewDestinationGoingToResetNavigation(&vec);
    if(!(r==true && h.called==1 && h.seen==&vec && h.seennav==&nav)){ printf("FAIL1\n"); return 1; }
    h.called=0; h.ret=false;
    r = nav.IsNewDestinationGoingToResetNavigation(&vec);
    if(!(r==false && h.called==1)){ printf("FAIL2\n"); return 1; }
    printf("OK_00c45160\n");
    return 0;
}