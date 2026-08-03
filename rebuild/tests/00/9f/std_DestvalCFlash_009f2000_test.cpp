#include <stdio.h>

// Standalone test: defines its own copy of the function under test (matching
// source_cpp exactly) plus recording stubs, then exercises it and prints
// pass_pattern on success.

typedef void (__stdcall *DestroyFlashFn)(void*);

extern "C" DestroyFlashFn __imp__DestroyFlashObj; // IAT slot at 0x0143fe0c
extern "C" char g_flashInstance_013caa70[1];       // literal object address 0x013caa70

struct CFlashAllocatorT;
struct CFlashObjT;

void __fastcall Dest_val_CFlash(CFlashAllocatorT* self, CFlashObjT* val)
{
    __imp__DestroyFlashObj(g_flashInstance_013caa70);
}

static int g_called = 0;
static void* g_lastArg = 0;

static void __stdcall fake_destroy(void* p)
{
    g_called = g_called + 1;
    g_lastArg = p;
}

DestroyFlashFn __imp__DestroyFlashObj = fake_destroy;
char g_flashInstance_013caa70[1] = { 0 };

int main()
{
    CFlashAllocatorT* dummySelf = (CFlashAllocatorT*)0x1234;
    CFlashObjT* dummyVal = (CFlashObjT*)0x5678;

    Dest_val_CFlash(dummySelf, dummyVal);

    if (g_called == 1 && g_lastArg == (void*)g_flashInstance_013caa70) {
        printf("DEST_VAL_CFLASH_OK\n");
        return 0;
    }

    printf("DEST_VAL_CFLASH_FAIL called=%d arg=%p expect=%p\n", g_called, g_lastArg, (void*)g_flashInstance_013caa70);
    return 1;
}