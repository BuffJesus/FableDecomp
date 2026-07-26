#include <cstdio>
static int g_helper_called = 0;
static void* g_freed = (void*)0;
extern "C" void __fastcall CActiveFile_Helper(void* self) { g_helper_called++; }
extern "C" void __cdecl CActiveFile_Free(void* p) { g_freed = p; }
extern "C" __declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    __asm {
        push esi
        mov esi, ecx
        call CActiveFile_Helper
        mov esi, dword ptr [esi]
        test esi, esi
        je done
        push esi
        call CActiveFile_Free
        pop ecx
    done:
        pop esi
        ret
    }
}
int main()
{
    void* p = (void*)0x1234;
    void* holder = p;
    void* self = &holder;
    g_helper_called = 0; g_freed = (void*)0;
    CActiveFile_OnReadFinished(self);
    if (g_helper_called == 1 && g_freed == p) {
        void* nullholder = (void*)0;
        g_helper_called = 0; g_freed = (void*)0;
        CActiveFile_OnReadFinished(&nullholder);
        if (g_helper_called == 1 && g_freed == (void*)0) {
            std::printf("CActiveFile_004518e1_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}