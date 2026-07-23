#include <cstdio>

static int g_cleanup_called = 0;
static void* g_freed = 0;

extern "C" void __cdecl engine_free(void* p) { g_freed = p; }
extern "C" void __fastcall CActiveFile_Cleanup(void* self) { g_cleanup_called++; }

__declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    __asm {
        push esi
        mov esi, ecx
        call CActiveFile_Cleanup
        mov esi, dword ptr [esi]
        test esi, esi
        je short L1
        push esi
        call engine_free
        pop ecx
    L1:
        pop esi
        ret
    }
}

int main()
{
    int zzq = 0;
    void* pzz = (void*)(&zzq);
    void* obj[3];

    obj[0] = pzz;
    g_cleanup_called = 0; g_freed = 0;
    CActiveFile_OnReadFinished(obj);
    if (g_cleanup_called != 1) { std::printf("FAIL cleanup\n"); return 1; }
    if (g_freed != pzz) { std::printf("FAIL freed\n"); return 1; }

    obj[0] = 0;
    g_cleanup_called = 0; g_freed = pzz;
    CActiveFile_OnReadFinished(obj);
    if (g_cleanup_called != 1) { std::printf("FAIL cleanup2\n"); return 1; }
    if (g_freed != pzz) { std::printf("FAIL freed2\n"); return 1; }

    std::printf("CActiveFile_004543c6_TEST PASS\n");
    return 0;
}