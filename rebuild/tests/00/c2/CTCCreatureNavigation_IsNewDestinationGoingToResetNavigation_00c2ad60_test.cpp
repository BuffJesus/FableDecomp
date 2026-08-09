
#include <cstdio>
struct C3DVector { float x,y,z; };
struct Nav;
struct Helper {
    int called;
    Nav* gotNav;
    C3DVector* gotDest;
    bool ret;
};
struct Nav {
    void* pad0;
    Helper* helper;
};

static bool call_slot6(Helper* h, Nav* nav, C3DVector* dest){
    h->called++;
    h->gotNav = nav;
    h->gotDest = dest;
    return h->ret;
}

// behavioural model of the leaf
static bool IsNewDestinationGoingToResetNavigation_model(Nav* self, C3DVector* dest){
    return call_slot6(self->helper, self, dest);
}

int main(){
    C3DVector d = {1,2,3};
    Helper h1 = {0, 0, 0, true};
    Nav n1; n1.pad0 = 0; n1.helper = &h1;
    bool r1 = IsNewDestinationGoingToResetNavigation_model(&n1, &d);
    if(!(r1==true && h1.called==1 && h1.gotNav==&n1 && h1.gotDest==&d)){ printf("FAIL1\n"); return 1; }

    Helper h2 = {0, 0, 0, false};
    Nav n2; n2.pad0 = 0; n2.helper = &h2;
    bool r2 = IsNewDestinationGoingToResetNavigation_model(&n2, &d);
    if(!(r2==false && h2.called==1 && h2.gotNav==&n2 && h2.gotDest==&d)){ printf("FAIL2\n"); return 1; }

    printf("OK_00c2ad60\n");
    return 0;
}