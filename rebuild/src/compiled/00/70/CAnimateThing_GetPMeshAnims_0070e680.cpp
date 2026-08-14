struct C3DAnimationBlendState;

struct CAnimateThingBase {
    void BaseGetPMeshAnims();  // 0x70e520, void thiscall, same this
};

struct CAnimateThing : public CAnimateThingBase {
    char pad[0x68];
    // 0x68: first blend-state member (interior ptr &+0x68)
    // 0x70: second blend-state member (interior ptr &+0x70)
    void GetPMeshAnims(C3DAnimationBlendState** outA, C3DAnimationBlendState** outB);
};

void CAnimateThing::GetPMeshAnims(C3DAnimationBlendState** outA,
                                  C3DAnimationBlendState** outB)
{
    BaseGetPMeshAnims();
    *outA = (C3DAnimationBlendState*)((char*)this + 0x68);
    *outB = (C3DAnimationBlendState*)((char*)this + 0x70);
}