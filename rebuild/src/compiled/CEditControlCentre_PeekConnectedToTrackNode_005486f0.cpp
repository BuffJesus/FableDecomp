struct CThingTrackNode;

struct CEditControlCentre {
    char pad0[0xc];
    char flag;         /* +0xc */
    char pad1[0x74 - 0xd];
    CThingTrackNode* nodeA;  /* +0x74 */
    char pad2[0x128 - 0x78];
    CThingTrackNode* nodeB;  /* +0x128 */
};

CThingTrackNode* __fastcall CEditControlCentre_PeekConnectedToTrackNode(CEditControlCentre* self)
{
    if (self->flag)
        return self->nodeB;
    return self->nodeA;
}