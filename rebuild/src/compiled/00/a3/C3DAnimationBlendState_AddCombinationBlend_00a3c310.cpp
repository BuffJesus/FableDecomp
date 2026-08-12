// C3DAnimationBlendState::AddCombinationBlend  (retail 0x00a3c310)
struct C3DAnimationStateAlpha;

// Subobject living at offset +0xc inside the member-4 object.
struct BlendSub {
    void Bar(C3DAnimationStateAlpha* a);   // declared only -> real call rel32
};

struct MemberFour {
    char pad[0xc];
    BlendSub sub;   // at +0xc
};

struct C3DAnimationBlendState {
    void* m_pad0;         // at +0
    MemberFour* m_four;   // at +4
    void Foo();           // declared only -> real call rel32
    void AddCombinationBlend(C3DAnimationStateAlpha* alpha);
};

void C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha* alpha)
{
    this->Foo();
    this->m_four->sub.Bar(alpha);
}