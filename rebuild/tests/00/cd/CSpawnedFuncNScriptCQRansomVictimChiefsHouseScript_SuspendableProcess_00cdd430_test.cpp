#include <stdio.h>

struct CSpawnedFunc {
    char pad0[5];
    unsigned char suspended;
    char pad6[0x2e];
    void (__fastcall *fn)(void*);
    void* arg;
};

static void* g_seen_arg;
static int g_called;

static void __fastcall model_fn(void* a) { g_seen_arg = a; g_called++; }

static void __fastcall SuspendableProcess_model(CSpawnedFunc* self)
{
    self->fn(self->arg);
    self->suspended = 1;
}

int main()
{
    CSpawnedFunc s;
    s.suspended = 0;
    s.fn = &model_fn;
    int dummy = 0;
    s.arg = &dummy;
    g_called = 0; g_seen_arg = 0;

    SuspendableProcess_model(&s);

    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_seen_arg != &dummy) { printf("FAIL arg\n"); return 1; }
    if (s.suspended != 1) { printf("FAIL suspended\n"); return 1; }

    printf("OK_00cdd430\n");
    return 0;
}