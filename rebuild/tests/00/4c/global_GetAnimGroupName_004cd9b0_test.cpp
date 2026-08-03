#include <cstdio>

// The function-under-test reads a global base pointer, then returns base + index (4-byte stride).
void* g_backing[8];
void** g_AnimGroupNameTable = g_backing;

__declspec(noinline) void** __fastcall GetAnimGroupName(int index)
{
    return g_AnimGroupNameTable + index;
}

int main()
{
    // Fill the backing table with distinct sentinels.
    for (int i = 0; i < 8; ++i) g_backing[i] = (void*)(0x1000 + i);

    // __fastcall passes the int arg in ecx; call directly (compiler honors __fastcall).
    void** p0 = GetAnimGroupName(0);
    void** p3 = GetAnimGroupName(3);
    void** p7 = GetAnimGroupName(7);

    if (p0 != &g_backing[0]) { std::printf("FAIL_0x004cd9b0 idx0\n"); return 1; }
    if (p3 != &g_backing[3]) { std::printf("FAIL_0x004cd9b0 idx3\n"); return 1; }
    if (p7 != &g_backing[7]) { std::printf("FAIL_0x004cd9b0 idx7\n"); return 1; }

    // Verify the returned pointer actually addresses the right entry.
    if (*p3 != (void*)(0x1000 + 3)) { std::printf("FAIL_0x004cd9b0 deref\n"); return 1; }

    // Pointer arithmetic must scale by 4 bytes.
    if ((char*)p3 - (char*)p0 != 12) { std::printf("FAIL_0x004cd9b0 stride\n"); return 1; }

    std::printf("OK_0x004cd9b0\n");
    return 0;
}