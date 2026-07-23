#include <cstdio>

struct CThingTrackNode;
struct CTrackNodeArg { char pad[8]; CThingTrackNode* node_at_8; };

CThingTrackNode* __stdcall CEditControlCentre_PeekConnectedToTrackNode(const CTrackNodeArg* self)
{
    if (self == 0)
        return 0;
    return self->node_at_8;
}

int main()
{
    CThingTrackNode* fake = (CThingTrackNode*)0x1234;
    CTrackNodeArg obj;
    obj.node_at_8 = fake;
    if (CEditControlCentre_PeekConnectedToTrackNode(0) != 0) { std::printf("FAIL null\n"); return 1; }
    if (CEditControlCentre_PeekConnectedToTrackNode(&obj) != fake) { std::printf("FAIL field\n"); return 1; }
    std::printf("CEditControlCentre_00cb13a0_TEST PASS\n");
    return 0;
}