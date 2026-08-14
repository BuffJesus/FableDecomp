#include <cstdio>

struct Obj {
    int m00, m04, m08, m0c, m10, m14, m18, m1c;
};

static int g_sub_called = 0;
static int g_base_called = 0;

void Sub8(void* p) { g_sub_called++; (void)p; }
void Base(Obj* self) { g_base_called++; (void)self; }

void FrameUpdate(Obj* self)
{
    int v = self->m1c + 7;
    if (v >= self->m10) {
        Sub8(&self->m08);
    }
    Base(self);
}

int main()
{
    // Case 1: m1c+7 >= m10 -> sub called, base always called
    Obj a = {0};
    a.m1c = 100; a.m10 = 100;   // 107 >= 100 -> sub called
    g_sub_called = g_base_called = 0;
    FrameUpdate(&a);
    if (g_sub_called != 1 || g_base_called != 1) { printf("FAIL1\n"); return 1; }

    // Case 2: m1c+7 < m10 -> sub NOT called, base called
    Obj b = {0};
    b.m1c = 0; b.m10 = 100; // 7 < 100 -> no sub
    g_sub_called = g_base_called = 0;
    FrameUpdate(&b);
    if (g_sub_called != 0 || g_base_called != 1) { printf("FAIL2\n"); return 1; }

    // Case 3: exact boundary m1c+7 == m10 -> jl false -> sub called
    Obj c = {0};
    c.m1c = 93; c.m10 = 100;  // 100 >= 100 true
    g_sub_called = g_base_called = 0;
    FrameUpdate(&c);
    if (g_sub_called != 1 || g_base_called != 1) { printf("FAIL3\n"); return 1; }

    // Case 4: signed compare
    Obj d = {0};
    d.m1c = -8; d.m10 = -5;  // -1 >= -5 true -> sub called
    g_sub_called = g_base_called = 0;
    FrameUpdate(&d);
    if (g_sub_called != 1 || g_base_called != 1) { printf("FAIL4\n"); return 1; }

    // Case 5: signed, no call
    Obj e = {0};
    e.m1c = -20; e.m10 = -5;  // -13 < -5 -> no sub
    g_sub_called = g_base_called = 0;
    FrameUpdate(&e);
    if (g_sub_called != 0 || g_base_called != 1) { printf("FAIL5\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}