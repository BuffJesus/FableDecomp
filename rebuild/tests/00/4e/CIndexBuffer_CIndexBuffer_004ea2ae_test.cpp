#include <stdio.h>

struct CIndexBuffer {
    unsigned int m_field0;
    unsigned int m_field4;
    unsigned int m_field8;
};

// Standalone transcription of the ctor under test (this=ecx via __fastcall,
// returns this in eax). Same body the source_cpp ctor lowers to.
__declspec(naked) void __fastcall CIndexBuffer_ctor()
{
    __asm {
        mov eax, ecx
        xor ecx, ecx
        mov [eax], ecx
        mov [eax+4], ecx
        mov [eax+8], ecx
        ret
    }
}

int main()
{
    CIndexBuffer obj;
    obj.m_field0 = 0xDEADBEEF;
    obj.m_field4 = 0xCAFEBABE;
    obj.m_field8 = 0x12345678;

    void* self = &obj;
    void* result = 0;

    __asm {
        mov ecx, self
        call CIndexBuffer_ctor
        mov result, eax
    }

    if (obj.m_field0 != 0) { printf("FAIL field0=%08x\n", obj.m_field0); return 1; }
    if (obj.m_field4 != 0) { printf("FAIL field4=%08x\n", obj.m_field4); return 1; }
    if (obj.m_field8 != 0) { printf("FAIL field8=%08x\n", obj.m_field8); return 1; }
    if (result != self)    { printf("FAIL ret mismatch\n"); return 1; }

    printf("OK_0x004ea2ae field0=%u field4=%u field8=%u ret==this\n",
           obj.m_field0, obj.m_field4, obj.m_field8);
    return 0;
}