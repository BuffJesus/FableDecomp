#include <stdio.h>
static int g_resolve = 0;
static int g_fetch = 0;
static int g_mid = 0;
static int g_out = 0;
extern "C" void *ResolveEditorAnimation(void *key) { ++g_resolve; return key ? &g_mid : 0; }
extern "C" void *FetchAnimationInfo(void *info) { ++g_fetch; return info == &g_mid ? &g_out : 0; }
extern "C" void *GetEditorAnimation(void *, void *key) { return FetchAnimationInfo(ResolveEditorAnimation(key)); }
int main() {
    int key = 1;
    if (GetEditorAnimation(0, &key) != &g_out) { printf("FAIL result\n"); return 1; }
    if (g_resolve != 1 || g_fetch != 1) { printf("FAIL calls\n"); return 1; }
    printf("PASS_00449a30\n");
    return 0;
}