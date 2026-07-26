#include <cstdio>
struct CTexture;
struct CDisplayManager;
static int g_calls = 0;
extern "C" void __declspec(naked) __fastcall DM_Grab(void){ __asm { ret 8 } }
extern "C" void __declspec(naked) __fastcall DM_Tail(void){ __asm { ret } }
__declspec(naked) CTexture* __fastcall CDisplayManager_CopyBackBufferToTexture(CDisplayManager* self)
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        lea eax, [ebp-1]
        push eax
        lea eax, [ebp-1]
        push eax
        mov esi, ecx
        call DM_Grab
        mov eax, esi
        pop esi
        leave
        ret
        jmp DM_Tail
    }
}
int main(){
    CDisplayManager* dm = (CDisplayManager*)0x12340000;
    CTexture* r = CDisplayManager_CopyBackBufferToTexture(dm);
    if ((void*)r == (void*)dm){
        std::printf("CDisplayManager_004534a6_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}