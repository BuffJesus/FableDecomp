#include <cstdio>

struct C3DVector { float x,y,z; };

struct Inner {
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void v4(){}
    virtual void v5(){}
    virtual void v6(){}
    virtual bool v7(void* a, C3DVector* v){ last_a=a; last_v=v; return ret; }
    static void* last_a;
    static C3DVector* last_v;
    static bool ret;
};
void* Inner::last_a = 0;
C3DVector* Inner::last_v = 0;
bool Inner::ret = false;

struct CTCCreatureNavigation {
    int f0;
    Inner* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* vec);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* vec)
{
    Inner* n = this->f4;
    return n->v7(this, vec);
}

int main()
{
    Inner inner;
    CTCCreatureNavigation nav;
    nav.f0 = 123;
    nav.f4 = &inner;
    C3DVector vec = {1.0f, 2.0f, 3.0f};

    Inner::ret = true;
    bool r = nav.IsNewDestinationGoingToResetNavigation(&vec);
    if (!r) { printf("FAIL true\n"); return 1; }
    if (Inner::last_a != (void*)&nav) { printf("FAIL a\n"); return 2; }
    if (Inner::last_v != &vec) { printf("FAIL v\n"); return 3; }

    Inner::ret = false;
    r = nav.IsNewDestinationGoingToResetNavigation(&vec);
    if (r) { printf("FAIL false\n"); return 4; }

    printf("OK_00c37f90\n");
    return 0;
}