struct C3DAnimationStateAlpha;

struct C3DAnimationBlendState {
    void helperA();                          // @0x744db0 __fastcall(this)
    void helperB(C3DAnimationStateAlpha* a); // @0x686910 __fastcall(this, a on stack)
    void AddCombinationBlend(C3DAnimationStateAlpha* a);
};

void C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha* a)
{
    this->helperA();
    this->helperB(a);
}