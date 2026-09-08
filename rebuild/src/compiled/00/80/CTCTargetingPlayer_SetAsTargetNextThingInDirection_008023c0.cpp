// Ego_r.pdb names all three state fields. The retail helper at 0x0049d870
// is used throughout gameplay as the current world-frame counter despite its
// stale propagated manifest label.
enum ETargetNextThingMethod { TARGET_NEXT_METHOD_DEFAULT = 0 };
extern unsigned long GetCurrentWorldFrame();

struct CTCTargetingPlayer {
    unsigned char _base_0x00[0x3c];
    bool TargetNextThing;
    unsigned char _pad_0x3d[3];
    ETargetNextThingMethod TargetNextThingMethod;
    unsigned long FrameSetAsTargetNextThing;

    void SetAsTargetNextThingInDirection(ETargetNextThingMethod method);
};

void CTCTargetingPlayer::SetAsTargetNextThingInDirection(
    ETargetNextThingMethod method) {
    TargetNextThing = true;
    TargetNextThingMethod = method;
    FrameSetAsTargetNextThing = GetCurrentWorldFrame();
}