#include <cstdio>

struct C3DAnimationStateAlpha { int tag; };

static int g_foo_calls = 0;
static C3DAnimationStateAlpha* g_bar_alpha = 0;
static void* g_bar_this = 0;

struct BlendSub {
    void Bar(C3DAnimationStateAlpha* a);
};

struct MemberFour {
    char pad[0xc];
    BlendSub sub;
};

struct C3DAnimationBlendState {
    void* m_pad0;
    MemberFour* m_four;
    void Foo();
    void AddCombinationBlend(C3DAnimationStateAlpha* alpha);
};

void C3DAnimationBlendState::Foo() { g_foo_calls++; }
void BlendSub::Bar(C3DAnimationStateAlpha* a) { g_bar_alpha = a; g_bar_this = this; }

void C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha* alpha)
{
    this->Foo();
    this->m_four->sub.Bar(alpha);
}

int main()
{
    MemberFour four;
    C3DAnimationBlendState st;
    st.m_four = &four;
    C3DAnimationStateAlpha alpha;
    alpha.tag = 0x1234;

    st.AddCombinationBlend(&alpha);

    bool ok = (g_foo_calls == 1)
           && (g_bar_alpha == &alpha)
           && (g_bar_this == (void*)&four.sub);

    if (ok) printf("BLENDSTATE_ADDCOMB_OK\n");
    else    printf("FAIL foo=%d\n", g_foo_calls);
    return ok ? 0 : 1;
}