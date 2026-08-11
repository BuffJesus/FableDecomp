#include <stdio.h>

struct C3DAnimationStateAlpha;

struct SubMember
{
    int addCount;
    C3DAnimationStateAlpha* lastAlpha;
    void AddAlpha(C3DAnimationStateAlpha* alpha);
};

struct C3DAnimationBlendState
{
    void* m_head;      // offset +0
    SubMember m_sub;   // offset +4
    int resetCount;

    void HelperReset();
    C3DAnimationBlendState* AddCombinationBlend(C3DAnimationStateAlpha* alpha);
};

void C3DAnimationBlendState::HelperReset()
{
    this->resetCount++;
    // record ordering: reset must happen before AddAlpha
    this->m_sub.addCount = this->m_sub.addCount; // no-op
    this->m_head = (void*)0x1;
}

void SubMember::AddAlpha(C3DAnimationStateAlpha* alpha)
{
    this->addCount++;
    this->lastAlpha = alpha;
}

int main()
{
    C3DAnimationBlendState bs;
    bs.m_head = 0;
    bs.m_sub.addCount = 0;
    bs.m_sub.lastAlpha = 0;
    bs.resetCount = 0;

    C3DAnimationStateAlpha* alpha = (C3DAnimationStateAlpha*)0xDEAD;
    C3DAnimationBlendState* r = bs.AddCombinationBlend(alpha);

    bool ok = true;
    if (r != &bs) ok = false;                 // returns this
    if (bs.resetCount != 1) ok = false;       // HelperReset ran once
    if (bs.m_sub.addCount != 1) ok = false;   // AddAlpha ran once
    if (bs.m_sub.lastAlpha != alpha) ok = false; // alpha forwarded
    if (bs.m_head == 0) ok = false;           // reset effect present

    printf(ok ? "COMBBLEND_OK\n" : "COMBBLEND_FAIL\n");
    return ok ? 0 : 1;
}