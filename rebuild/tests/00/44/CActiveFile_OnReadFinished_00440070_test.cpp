#include <stdio.h>
struct ActiveFile { void *token; };
static int g_released = 0;
static void *g_last = 0;
extern "C" void ReleaseReadToken(void *p) { ++g_released; g_last = p; }
extern "C" void OnReadFinished(ActiveFile *self) { if (self->token) ReleaseReadToken(self->token); }
int main() {
    ActiveFile f; int token = 9;
    f.token = 0; OnReadFinished(&f);
    if (g_released != 0) { printf("FAIL null release\n"); return 1; }
    f.token = &token; OnReadFinished(&f);
    if (g_released != 1 || g_last != &token) { printf("FAIL release path\n"); return 1; }
    printf("PASS_00440070\n");
    return 0;
}