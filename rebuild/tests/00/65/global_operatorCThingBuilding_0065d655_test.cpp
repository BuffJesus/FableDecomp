#include <cstdio>

struct CThingBuilding;

struct CIntelligentPointer {
    CThingBuilding* p;
};

static CThingBuilding* __fastcall IP_Get(CIntelligentPointer* self) {
    return self->p;
}

static bool __fastcall eq(CIntelligentPointer* a, CIntelligentPointer* b) {
    return IP_Get(a) == IP_Get(b);
}

int main() {
    CThingBuilding* t1 = (CThingBuilding*)0x1000;
    CThingBuilding* t2 = (CThingBuilding*)0x2000;
    CIntelligentPointer A, B, C;
    A.p = t1; B.p = t1; C.p = t2;
    bool r1 = eq(&A, &B); // same -> true
    bool r2 = eq(&A, &C); // diff -> false
    if (r1 == true && r2 == false) {
        printf("EQ_OK\n");
    } else {
        printf("EQ_FAIL %d %d\n", (int)r1, (int)r2);
    }
    return 0;
}