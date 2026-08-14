#include <stdio.h>

struct SubObjA { int v; };
struct Recorder { SubObjA* lastArg; int addCalls; };
struct SubListF2 { Recorder* rec; void Add(SubObjA* a) { rec->lastArg = a; rec->addCalls++; } };
struct Base {
    char      pad00[0x0c];
    SubObjA   subA;
    char      padA[0xf8 - 0x10];
    int       stateF8;
    SubListF2 listFc;
};
static void AddTrigger_model(Base* self) { self->listFc.Add(&self->subA); self->stateF8 = 5; }
int main() {
    Base b; b.subA.v = 0x1234; b.stateF8 = 0;
    Recorder r; r.lastArg = 0; r.addCalls = 0; b.listFc.rec = &r;
    AddTrigger_model(&b);
    bool ok = (r.addCalls == 1) && (r.lastArg == &b.subA) && (b.stateF8 == 5);
    Base probe;
    unsigned long offA  = (unsigned long)((char*)&probe.subA    - (char*)&probe);
    unsigned long offF8 = (unsigned long)((char*)&probe.stateF8 - (char*)&probe);
    unsigned long offFc = (unsigned long)((char*)&probe.listFc  - (char*)&probe);
    ok = ok && offA == 0x0c && offF8 == 0xf8 && offFc == 0xfc;
    if (ok) printf("AddTrigger_PASS\n");
    else    printf("AddTrigger_FAIL a=%d s=%d oA=%lu oF8=%lu oFc=%lu\n", r.addCalls, b.stateF8, offA, offF8, offFc);
    return 0;
}