#include <stdio.h>

// Behavioural model of _Cons_val_ctor: null-check first arg.
// If arg0 == null: return immediately (no forward).
// Else: forward to target(arg0,arg1,arg2).

static int g_forwarded = 0;
static void* g_seen = 0;

static void target_impl(void* a, void* b, void* c)
{
    g_forwarded = 1;
    g_seen = a;
}

static void (*g_target)(void*, void*, void*) = target_impl;

// model
static void model(void* a, void* b, void* c)
{
    if (a == 0) return;      // ret 4
    g_target(a, b, c);       // tail jmp
}

int main()
{
    // null path: no forward
    g_forwarded = 0; g_seen = (void*)0xDEAD;
    model(0, (void*)1, (void*)2);
    if (g_forwarded != 0) { printf("FAIL null path\n"); return 1; }

    // non-null path: forwards, arg preserved
    g_forwarded = 0; g_seen = 0;
    int obj = 7;
    model(&obj, (void*)5, (void*)6);
    if (g_forwarded != 1) { printf("FAIL nonnull path\n"); return 2; }
    if (g_seen != &obj) { printf("FAIL arg preserve\n"); return 3; }

    printf("OK_00c0bcc0\n");
    return 0;
}