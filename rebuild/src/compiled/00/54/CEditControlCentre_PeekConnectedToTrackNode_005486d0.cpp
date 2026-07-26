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