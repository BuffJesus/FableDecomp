#include <stdio.h>

struct C3DAnimationBlendState;

static int g_baseCalled = 0;

struct CAnimateThingBase {
    void BaseGetPMeshAnims() { g_baseCalled = 1; }
};

struct CAnimateThing : public CAnimateThingBase {
    char pad[0x68];
    char more[0x20];
    void GetPMeshAnims(C3DAnimationBlendState** outA, C3DAnimationBlendState** outB);
};

void CAnimateThing::GetPMeshAnims(C3DAnimationBlendState** outA,
                                  C3DAnimationBlendState** outB)
{
    BaseGetPMeshAnims();
    *outA = (C3DAnimationBlendState*)((char*)this + 0x68);
    *outB = (C3DAnimationBlendState*)((char*)this + 0x70);
}

int main(void) {
    CAnimateThing t;
    C3DAnimationBlendState* a = 0;
    C3DAnimationBlendState* b = 0;
    t.GetPMeshAnims(&a, &b);
    int ok = 1;
    if (!g_baseCalled) ok = 0;
    if ((char*)a != (char*)&t + 0x68) ok = 0;
    if ((char*)b != (char*)&t + 0x70) ok = 0;
    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL\n");
    return 0;
}