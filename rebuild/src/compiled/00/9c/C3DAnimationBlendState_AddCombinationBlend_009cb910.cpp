// C3DAnimationBlendState::AddCombinationBlend
// __fastcall(C3DAnimationBlendState* this, C3DAnimationStateAlpha* alpha) -> returns this
// retail 009cb910:
//   push esi; mov esi,ecx; call 0x99e4b0 (thiscall on this)
//   mov eax,[esp+8]; push eax; lea ecx,[esi+4]; call 0x99ec30 (thiscall on this+4, arg=alpha)
//   mov eax,esi; pop esi; ret 4

struct C3DAnimationStateAlpha;

struct SubMember
{
    // method at 0x99ec30
    void AddAlpha(C3DAnimationStateAlpha* alpha);
};

struct C3DAnimationBlendState
{
    void* m_head;      // offset +0 (this-relative helper target uses this directly)
    SubMember m_sub;   // offset +4

    void HelperReset();   // method at 0x99e4b0 (thiscall on this)

    C3DAnimationBlendState* AddCombinationBlend(C3DAnimationStateAlpha* alpha);
};

C3DAnimationBlendState* C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha* alpha)
{
    HelperReset();
    m_sub.AddAlpha(alpha);
    return this;
}