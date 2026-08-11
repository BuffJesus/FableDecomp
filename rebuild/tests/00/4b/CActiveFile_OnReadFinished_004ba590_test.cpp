#include <stdio.h>

struct CActiveFile { void* firstMember; };

static int g_baseCalls = 0;
static void* g_freed = (void*)0;
static int g_freeCalls = 0;

void __fastcall BaseReadDone(CActiveFile* self) { g_baseCalls++; }
void FreeHelper(void* p) { g_freed = p; g_freeCalls++; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    // Case 1: firstMember non-null -> base called, helper called with member
    CActiveFile a;
    int dummy = 42;
    a.firstMember = &dummy;
    g_baseCalls = 0; g_freeCalls = 0; g_freed = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (g_baseCalls != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freeCalls != 1) { printf("FAIL free1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL freed1\n"); return 1; }

    // Case 2: firstMember null -> base called, helper NOT called
    CActiveFile b;
    b.firstMember = (void*)0;
    g_baseCalls = 0; g_freeCalls = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_baseCalls != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freeCalls != 0) { printf("FAIL free2\n"); return 1; }

    printf("004ba590_TEST PASS\n");
    return 0;
}