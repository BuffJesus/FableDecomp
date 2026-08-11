// Detects SSE support via CPUID and records it in a global flag.
unsigned char g_HasSSE = 0;

void GFInitVectorMath()
{
    __asm
    {
        mov     eax, 1
        cpuid
        test    edx, 0x2000000
        je      no_sse
        mov     byte ptr [g_HasSSE], 1
    no_sse:
    }
}