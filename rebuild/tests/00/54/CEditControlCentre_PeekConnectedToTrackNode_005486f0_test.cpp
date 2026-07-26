#include <cstdio>

struct CThingTrackNode;

struct CEditControlCentre {
    char pad0[0xc];
    char flag;
    char pad1[0x74 - 0xd];
    CThingTrackNode* nodeA;
    char pad2[0x128 - 0x78];
    CThingTrackNode* nodeB;
};

CThingTrackNode* __fastcall CEditControlCentre_PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->flag)
        return self->nodeB;
    return self->nodeA;
}

int main()
{
    CEditControlCentre e;
    CThingTrackNode* a = (CThingTrackNode*)0x1111;
    CThingTrackNode* b = (CThingTrackNode*)0x2222;
    e.nodeA = a;
    e.nodeB = b;
    e.flag = 0;
    if (CEditControlCentre_PeekConnectedToTrackNode(&e) != a) { std::printf("FAIL flag0\n"); return 1; }
    e.flag = 1;
    if (CEditControlCentre_PeekConnectedToTrackNode(&e) != b) { std::printf("FAIL flag1\n"); return 1; }
    std::printf("CEditControlCentre_005486f0_TEST PASS\n");
    return 0;
}