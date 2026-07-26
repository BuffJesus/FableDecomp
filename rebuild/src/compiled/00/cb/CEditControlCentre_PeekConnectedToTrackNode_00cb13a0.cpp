struct CThingTrackNode;
struct CTrackNodeArg { char pad[8]; CThingTrackNode* node_at_8; };

CThingTrackNode* __stdcall CEditControlCentre_PeekConnectedToTrackNode(const CTrackNodeArg* self)
{
    if (self == 0)
        return 0;
    return self->node_at_8;
}