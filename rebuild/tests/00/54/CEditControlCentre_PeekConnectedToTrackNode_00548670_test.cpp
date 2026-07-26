// Standalone behaviour test for CEditControlCentre::PeekConnectedToTrackNode
#include <cstdio>

struct CThingTrackNode;

struct CEditControlCentre {
    char _pad0[0x0c];
    unsigned char use_connected;    // 0x0c
    char _pad1[0x38 - 0x0d];
    CThingTrackNode* default_node;  // 0x38
    char _pad2[0xf4 - 0x3c];
    CThingTrackNode* connected_node;// 0xf4
};

CThingTrackNode* __fastcall CEditControlCentre_PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->use_connected)
        return self->connected_node;
    return self->default_node;
}

int main()
{
    // Offset sanity
    typedef char _ck_off_flag[(0x0c == (int)((char*)&((CEditControlCentre*)0)->use_connected  - (char*)0)) ? 1 : -1];
    typedef char _ck_off_def [(0x38 == (int)((char*)&((CEditControlCentre*)0)->default_node   - (char*)0)) ? 1 : -1];
    typedef char _ck_off_con [(0xf4 == (int)((char*)&((CEditControlCentre*)0)->connected_node - (char*)0)) ? 1 : -1];
    (void)sizeof(_ck_off_flag); (void)sizeof(_ck_off_def); (void)sizeof(_ck_off_con);

    CThingTrackNode* A = (CThingTrackNode*)0x11110000; // default
    CThingTrackNode* B = (CThingTrackNode*)0x22220000; // connected

    CEditControlCentre obj;
    obj.default_node   = A;
    obj.connected_node = B;

    obj.use_connected = 0;
    CThingTrackNode* r0 = CEditControlCentre_PeekConnectedToTrackNode(&obj);

    obj.use_connected = 1;
    CThingTrackNode* r1 = CEditControlCentre_PeekConnectedToTrackNode(&obj);

    // nonzero-but-not-1 flag must still select connected
    obj.use_connected = 0x7f;
    CThingTrackNode* r2 = CEditControlCentre_PeekConnectedToTrackNode(&obj);

    if (r0 == A && r1 == B && r2 == B) {
        printf("CEDITCONTROLCENTRE_00548670_TEST PASS\n");
        return 0;
    }
    printf("FAIL r0=%p r1=%p r2=%p\n", (void*)r0, (void*)r1, (void*)r2);
    return 1;
}