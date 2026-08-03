#include <stdio.h>

struct CEditControlCentre {
    char pad0[0xC];
    unsigned char connectedFlag;
    char pad1[0x48 - 0xC - 1];
    void* trackNodeA;
    char pad2[0x104 - 0x48 - 4];
    void* trackNodeB;
};

void* __fastcall PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->connectedFlag)
        return self->trackNodeB;
    return self->trackNodeA;
}

static void* callit(CEditControlCentre* obj)
{
    void* r;
    void* fn = (void*)&PeekConnectedToTrackNode;
    __asm {
        mov ecx, obj
        call fn
        mov r, eax
    }
    return r;
}

int main()
{
    CEditControlCentre a;
    int nodeA = 111;
    int nodeB = 222;
    a.trackNodeA = &nodeA;
    a.trackNodeB = &nodeB;

    a.connectedFlag = 0;
    void* r0 = callit(&a);

    a.connectedFlag = 1;
    void* r1 = callit(&a);

    if (r0 == &nodeA && r1 == &nodeB) {
        printf("OK_0x005486b0\n");
        return 0;
    }
    printf("BAD r0=%p r1=%p\n", r0, r1);
    return 1;
}