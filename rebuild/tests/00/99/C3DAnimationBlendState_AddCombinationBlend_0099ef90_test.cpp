#include <cstdio>

struct C3DAnimationStateAlpha { int id; };

static int g_log = 0;
static C3DAnimationStateAlpha* g_gotArg = 0;

struct Inner {
    void addAlpha(C3DAnimationStateAlpha* a) { g_log = g_log * 10 + 2; g_gotArg = a; }
};

struct C3DAnimationBlendState {
    Inner* pInner;
    void helperA() { g_log = g_log * 10 + 1; }
};

void __fastcall AddCombinationBlend(C3DAnimationBlendState* self, int /*edx*/, C3DAnimationStateAlpha* a)
{
    self->helperA();
    self->pInner->addAlpha(a);
}

int main() {
    Inner inner;
    C3DAnimationBlendState st; st.pInner = &inner;
    C3DAnimationStateAlpha alpha; alpha.id = 77;
    AddCombinationBlend(&st, 0, &alpha);
    if (g_log == 12 && g_gotArg == &alpha) {
        printf("0099ef90_TEST PASS\n");
    } else {
        printf("FAIL log=%d\n", g_log);
    }
    return 0;
}