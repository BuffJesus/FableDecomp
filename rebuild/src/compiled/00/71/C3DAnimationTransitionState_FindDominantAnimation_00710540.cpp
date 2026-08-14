// C3DAnimationTransitionState::FindDominantAnimation @ 0x00710540
struct C3DAnimationState;

struct IAnim {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual const C3DAnimationState& FindDominant() = 0; // slot 3 (offset 0xc)
};

struct C3DAnimationTransitionState {
    char pad0[0x8];
    IAnim* a8;    // +0x08
    IAnim* ac;    // +0x0c
    float f10;    // +0x10
};

extern const float g_threshold; // [0x122f59c]

const C3DAnimationState& __fastcall FindDominantAnimation(C3DAnimationTransitionState* self)
{
    if (self->f10 <= g_threshold)
        return self->a8->FindDominant();
    return self->ac->FindDominant();
}