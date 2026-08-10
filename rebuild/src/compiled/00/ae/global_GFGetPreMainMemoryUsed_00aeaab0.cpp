// GFGetPreMainMemoryUsed @ 00aeaab0
// mov eax, dword ptr [0x13d2e0c]; ret
extern "C" long g_GFPreMainMemoryUsed;

long __fastcall GFGetPreMainMemoryUsed()
{
    return g_GFPreMainMemoryUsed;
}