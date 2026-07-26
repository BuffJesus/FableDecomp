#include <cstdio>

struct CThingTrackNode;
struct CEditControlCentre {
    char pad0[0xc];
    char m_flag;
    char pad1[0x108 - 0xd];
    CThingTrackNode* m_connectedNode;
};

CThingTrackNode* __fastcall CEditControlCentre_PeekConnectedToTrackNode(const CEditControlCentre* self)
{
    if (self->m_flag)
        return self->m_connectedNode;
    return *(CThingTrackNode* const*)((const char*)self + 0x4c);
}

int main()
{
    char buf[0x200];
    for (int i = 0; i < 0x200; ++i) buf[i] = 0;
    CEditControlCentre* obj = (CEditControlCentre*)buf;
    CThingTrackNode* connNode = (CThingTrackNode*)0x11112222;
    CThingTrackNode* node4c = (CThingTrackNode*)0x33334444;
    obj->m_connectedNode = connNode;
    *(CThingTrackNode**)(buf + 0x4c) = node4c;

    obj->m_flag = 1;
    if (CEditControlCentre_PeekConnectedToTrackNode(obj) != connNode) {
        std::printf("FAIL flag-set\n");
        return 1;
    }
    obj->m_flag = 0;
    if (CEditControlCentre_PeekConnectedToTrackNode(obj) != node4c) {
        std::printf("FAIL flag-clear\n");
        return 1;
    }
    std::printf("CEditControlCentre_005486d0_TEST PASS\n");
    return 0;
}