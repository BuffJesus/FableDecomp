#include <stdio.h>
struct ActiveFile { void *token; };
static int g_prelude = 0;
static int g_release = 0;
static void *g_last = 0;
extern "C" void FinishReadPrelude(void) { ++g_prelude; }
extern "C" void ReleaseReadToken(void *p) { ++g_release; g_last = p; }
extern "C" void OnReadFinished(ActiveFile *self) { FinishReadPrelude(); if (self->token) ReleaseReadToken(self->token); }
int main() {
    ActiveFile f; int token = 4;
    f.token = 0; OnReadFinished(&f);
    if (g_prelude != 1 || g_release != 0) { printf("FAIL null path\n"); return 1; }
    f.token = &token; OnReadFinished(&f);
    if (g_prelude != 2 || g_release != 1 || g_last != &token) { printf("FAIL release path\n"); return 1; }
    printf("PASS_00440d40\n");
    return 0;
}