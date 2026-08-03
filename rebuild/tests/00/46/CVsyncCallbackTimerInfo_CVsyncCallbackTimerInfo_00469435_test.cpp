#include <stdio.h>

struct CVsyncCallbackTimerInfo {
    void* p0;      // +0x00
    void* p4;      // +0x04
    float f8;      // +0x08
    void* pc;      // +0x0c
    float f10;     // +0x10
};

// Function name equals the struct tag; reference the struct with the 'struct'
// keyword elsewhere so the name resolves to the ctor for the __asm call.
__declspec(naked) struct CVsyncCallbackTimerInfo* __fastcall CVsyncCallbackTimerInfo(struct CVsyncCallbackTimerInfo* /*self ecx*/)
{
    __asm {
        mov  eax, ecx
        fldz
        xor  ecx, ecx
        mov  dword ptr [eax], ecx
        fstp dword ptr [eax+8]
        fldz
        mov  dword ptr [eax+4], ecx
        fstp dword ptr [eax+0x10]
        mov  dword ptr [eax+0xc], ecx
        ret
    }
}

int main()
{
    struct CVsyncCallbackTimerInfo obj;
    // Pre-fill with garbage so we can prove the ctor zeroed every field.
    obj.p0  = (void*)0xDEADBEEF;
    obj.p4  = (void*)0xCAFEBABE;
    obj.f8  = 3.5f;
    obj.pc  = (void*)0x12345678;
    obj.f10 = -7.25f;

    struct CVsyncCallbackTimerInfo* self = &obj;
    struct CVsyncCallbackTimerInfo* ret = 0;
    __asm {
        mov ecx, self
        call CVsyncCallbackTimerInfo
        mov  ret, eax
    }

    int ok = 1;
    if (ret != self)     ok = 0;
    if (obj.p0  != 0)    ok = 0;
    if (obj.p4  != 0)    ok = 0;
    if (obj.f8  != 0.0f) ok = 0;
    if (obj.pc  != 0)    ok = 0;
    if (obj.f10 != 0.0f) ok = 0;

    if (ok) {
        printf("OK_0x00469435 ctor zeroed all fields, returned this\n");
        return 0;
    }
    printf("FAIL_0x00469435 p0=%p p4=%p f8=%f pc=%p f10=%f\n",
           obj.p0, obj.p4, obj.f8, obj.pc, obj.f10);
    return 1;
}