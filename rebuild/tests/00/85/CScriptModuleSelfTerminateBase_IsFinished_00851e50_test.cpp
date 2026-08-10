#include <cstdio>

static int g_elapsed;
int GetElapsed_model() { return g_elapsed; }

struct M { char pad[0x50]; int thr; };

bool IsFinished_model(M* self) { return GetElapsed_model() > self->thr; }

int main() {
    M m; m.thr = 100;
    g_elapsed = 50;  if (IsFinished_model(&m)) { printf("FAIL\n"); return 1; }
    g_elapsed = 150; if (!IsFinished_model(&m)) { printf("FAIL\n"); return 1; }
    g_elapsed = 100; if (IsFinished_model(&m)) { printf("FAIL\n"); return 1; } /* not > */
    printf("OK_00851e50\n");
    return 0;
}