// CMovie::FreeTextures @ 0x00547b90
extern "C" void* g_movieTexture;   // ds:0x013b8ac0
extern "C" void* g_movieDevice;    // ds:0x013b8ab8

__declspec(naked) void FreeTextures()
{
    __asm {
        mov     eax, dword ptr [g_movieTexture]
        test    eax, eax
        je      done
        mov     ecx, dword ptr [g_movieDevice]
        mov     edx, dword ptr [ecx]
        push    eax
        call    dword ptr [edx+0x118]
        mov     dword ptr [g_movieTexture], 0
    done:
        ret
    }
}