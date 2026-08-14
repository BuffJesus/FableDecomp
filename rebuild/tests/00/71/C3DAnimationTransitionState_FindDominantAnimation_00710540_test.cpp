#include <cstdio>

struct C3DAnimationState { int id; };

struct IAnim {
    C3DAnimationState st;
    virtual const C3DAnimationState& FindDominant() { return st; }
};

struct TransitionState {
    char pad0[0x8];
    IAnim* a8;
    IAnim* ac;
    float f10;
};

static float g_threshold = 1.0f;

static const C3DAnimationState& model(TransitionState* self)
{
    if (self->f10 <= g_threshold)
        return self->a8->FindDominant();
    return self->ac->FindDominant();
}

int main()
{
    IAnim A, B;
    A.st.id = 111; B.st.id = 222;
    TransitionState s;
    s.a8 = &A; s.ac = &B;

    s.f10 = 2.0f; // > 1 -> ac -> B (222)
    const C3DAnimationState& r1 = model(&s);
    s.f10 = 0.5f; // not > 1 -> a8 -> A (111)
    const C3DAnimationState& r2 = model(&s);

    if (r1.id == 222 && r2.id == 111)
        printf("BEHAVIOR_OK\n");
    else
        printf("BEHAVIOR_FAIL %d %d\n", r1.id, r2.id);
    return 0;
}