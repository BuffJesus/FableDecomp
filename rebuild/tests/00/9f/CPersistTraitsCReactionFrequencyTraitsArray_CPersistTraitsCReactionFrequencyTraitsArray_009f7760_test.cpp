#include <cstdio>

struct Param { void* m0; };

struct Obj {
    void* seen;
    void* callee(void* arg) { seen = arg; return this; }
};

Obj* __fastcall dtor_model(Obj* self, void* /*edx*/, Param* p)
{
    self->callee(p->m0);
    return self;
}

int main()
{
    Obj o;
    o.seen = 0;
    int marker = 42;
    void* markerPtr = static_cast<void*>(&marker);
    Param p;
    p.m0 = markerPtr;
    Obj* objPtr = static_cast<Obj*>(&o);
    Param* pPtr = static_cast<Param*>(&p);
    Obj* r = dtor_model(objPtr, 0, pPtr);
    if (r == objPtr && o.seen == markerPtr) {
        printf("PARITY_OK_009f7760\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}