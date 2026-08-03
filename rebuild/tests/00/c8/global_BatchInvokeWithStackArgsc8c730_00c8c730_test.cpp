// Behaviour test for Batch_InvokeWithStackArgs @ 0x00C8C730.
// Self-contained: it embeds the naked function-under-test, stubs every engine
// call it references (so the test links standalone), and drives the two
// call-light early-return paths that are fully deterministic without modelling
// the alloca/usercall main body:
//   (A) state->count < 2            -> returns -0x83 (0xFFFFFF7D)
//   (B) count>=2 && first call != 0 -> returns that first-call result verbatim
#include <cstdio>

// --- controllable engine stubs (only CreatureHitNotification is reached, in B) ---
static int g_notifyResult = 0;
extern "C" {
    int  CreatureHitNotification_WaitForCompletion() { return g_notifyResult; }
    int  Indexed_GetElementOffsetSafe() { return 0; }
    int  GetNextRegionOnRouteTo()       { return 0; }
    int  DrawGetWorldMap()              { return 0; }
    int  GetEntryDataSize()             { return 0; }
    void VertexBuffer_FillData()        {}
    int  VertexBuffer_WaitAndAllocate() { return 0; }
    void Buffer_SwapElements()          {}
    void Audio_ConvolveChannels()       {}
    // Referenced by the two _alloca sites; never reached by A/B, but must link.
    void __chkstk()                     {}
}

// --- exact copy of the landed naked transcription (the function under test) ---
__declspec(naked) int Batch_InvokeWithStackArgs_c8c730()
{
    __asm {
        push    ebp
        mov     ebp, esp
        sub     esp, 0x1c
        push    ebx
        push    esi
        mov     esi, eax
        cmp     dword ptr [esi+0x58], 2
        push    edi
        jge     cont
        mov     eax, 0xffffff7d
        lea     esp, [ebp-0x28]
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     0xc
cont:
        call    CreatureHitNotification_WaitForCompletion
        test    eax, eax
        jne     done
        or      edx, 0xffffffff
        mov     ecx, esi
        call    Indexed_GetElementOffsetSafe
        mov     ebx, eax
        mov     dword ptr [ebp-0x14], ebx
        call    GetNextRegionOnRouteTo
        mov     edi, dword ptr [ebx+0x4]
        inc     eax
        xor     edx, edx
        mov     ecx, ebx
        mov     dword ptr [ebp-0x10], eax
        call    GetEntryDataSize
        mov     ecx, dword ptr [ebp-0x10]
        mov     ebx, eax
        sar     ebx, cl
        lea     ecx, [esi+0x1e0]
        xor     edx, edx
        mov     dword ptr [ebp-0x4], ecx
        call    DrawGetWorldMap
        mov     dword ptr [ebp-0x1c], eax
        lea     eax, [edi*4+0x0]
        add     eax, 3
        and     eax, 0xfffffffc
        call    __chkstk
        test    edi, edi
        mov     dword ptr [ebp-0x8], esp
        mov     dword ptr [ebp-0xc], 0
        jle     afterloop
        _emit 0x8d
        _emit 0x49
        _emit 0x00
    }
    __asm {
loophead:
        lea     eax, [ebx*4+0x0]
        add     eax, 3
        and     eax, 0xfffffffc
        call    __chkstk
        mov     eax, dword ptr [ebp-0xc]
        mov     edx, dword ptr [ebp-0x8]
        mov     ecx, esp
        mov     dword ptr [edx+eax*4], ecx
        inc     eax
        cmp     eax, edi
        mov     dword ptr [ebp-0xc], eax
        jl      loophead
afterloop:
        mov     eax, dword ptr [ebp-0x8]
        mov     ecx, dword ptr [ebp-0x4]
        mov     edx, dword ptr [ebp-0x14]
        push    ebx
        push    eax
        push    ecx
        push    edx
        push    esi
        call    VertexBuffer_FillData
        fld     qword ptr [ebp+0x8]
        sub     esp, 8
        mov     ecx, esi
        fstp    qword ptr [esp]
        call    dword ptr [ebp+0x10]
        test    eax, eax
        jne     done
        call    VertexBuffer_WaitAndAllocate
        test    eax, eax
        jne     done
        or      edx, 0xffffffff
        mov     ecx, esi
        call    Indexed_GetElementOffsetSafe
        mov     ecx, dword ptr [eax+0x4]
        mov     dword ptr [ebp+0xc], ecx
        xor     edx, edx
        mov     ecx, eax
        call    GetEntryDataSize
        mov     ecx, dword ptr [ebp-0x10]
        mov     esi, eax
        sar     esi, cl
        mov     ecx, dword ptr [ebp-0x4]
        xor     edx, edx
        call    DrawGetWorldMap
        mov     ecx, dword ptr [ebp-0x4]
        lea     edx, [ebp-0x18]
        mov     dword ptr [ebp+0x10], eax
        call    Buffer_SwapElements
        mov     edx, dword ptr [ebp+0x10]
        mov     eax, dword ptr [ebp+0xc]
        mov     ecx, dword ptr [ebp-0x8]
        push    edx
        mov     edx, dword ptr [ebp-0x18]
        push    eax
        push    edi
        push    ecx
        push    edx
        mov     edx, dword ptr [ebp-0x1c]
        mov     eax, esi
        mov     ecx, ebx
        call    Audio_ConvolveChannels
        xor     eax, eax
done:
        lea     esp, [ebp-0x28]
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     0xc
    }
}

// this=EAX entry with a (double, callbackPtr) 12-byte arg frame cleaned by ret 0xC
static int drive(void* state, unsigned lo, unsigned hi, void* cb)
{
    int r;
    __asm {
        mov     eax, cb
        push    eax
        mov     eax, hi
        push    eax
        mov     eax, lo
        push    eax
        mov     eax, state
        call    Batch_InvokeWithStackArgs_c8c730
        mov     r, eax
    }
    return r;
}

int main()
{
    unsigned char state[0x200];
    for (int i = 0; i < 0x200; ++i) state[i] = 0;
    int* count = reinterpret_cast<int*>(state + 0x58);

    // (A) count < 2 -> -0x83, no engine calls reached
    *count = 1;
    g_notifyResult = 0;
    int a = drive(state, 0, 0, 0);
    if (a != (int)0xFFFFFF7D) {
        std::printf("Batch_InvokeWithStackArgs_c8c730_TEST FAIL(A): got %d want %d\n", a, (int)0xFFFFFF7D);
        return 1;
    }

    // (B) count >= 2, first call returns sentinel -> returned verbatim
    *count = 5;
    g_notifyResult = 0x1234;
    int b = drive(state, 0, 0, 0);
    if (b != 0x1234) {
        std::printf("Batch_InvokeWithStackArgs_c8c730_TEST FAIL(B): got %d want %d\n", b, 0x1234);
        return 1;
    }

    std::printf("Batch_InvokeWithStackArgs_c8c730_TEST PASS\n");
    return 0;
}