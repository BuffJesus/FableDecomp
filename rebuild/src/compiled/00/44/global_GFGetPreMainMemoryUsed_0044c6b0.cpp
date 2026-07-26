extern long g_GFPreMainMemoryUsed;

long __fastcall GFGetPreMainMemoryUsed()
{
    return g_GFPreMainMemoryUsed;
}