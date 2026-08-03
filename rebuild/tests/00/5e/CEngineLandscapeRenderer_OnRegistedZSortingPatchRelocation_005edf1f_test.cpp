#include <stdio.h>

struct CEngineLandscapeRenderer {
    char pad[0x148];
    int* m_zSortingPatchArray; // +0x148
};

// __fastcall: this in ecx, args (index, value) on stack, callee cleans 8 (ret 8).
void __fastcall OnRegistedZSortingPatchRelocation(CEngineLandscapeRenderer* self, void* /*edx*/, int index, int value)
{
    self->m_zSortingPatchArray[index] = value;
}

int main()
{
    CEngineLandscapeRenderer obj;
    for (int i = 0; i < 0x148; ++i) obj.pad[i] = 0;
    int arr[8];
    for (int i = 0; i < 8; ++i) arr[i] = -1;
    obj.m_zSortingPatchArray = arr;

    CEngineLandscapeRenderer* pobj = &obj;
    int idx = 3;
    int val = 0x1234;

    // Invoke via a raw __fastcall shim: this=ecx, push value then index.
    void* fn = (void*)&OnRegistedZSortingPatchRelocation;
    __asm {
        push val
        push idx
        mov  ecx, pobj
        call fn
    }

    if (arr[3] != 0x1234) { printf("FAIL arr[3]=%d\n", arr[3]); return 1; }
    if (arr[0] != -1 || arr[2] != -1 || arr[4] != -1) { printf("FAIL neighbor clobber\n"); return 1; }

    // second store at a different index
    idx = 0; val = 0x77;
    __asm {
        push val
        push idx
        mov  ecx, pobj
        call fn
    }
    if (arr[0] != 0x77) { printf("FAIL arr[0]=%d\n", arr[0]); return 1; }

    printf("OK_0x005edf1f\n");
    return 0;
}