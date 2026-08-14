#include <cstdio>
#include <cstring>

struct CNavRegion { char pad[384]; };

// Standalone behaviour model: object is a raw byte buffer; fields by offset.
static char* model(char* self)
{
    char flag = self[0x33c];
    if (flag)
        return 0;
    int index = *reinterpret_cast<int*>(self + 0x318);
    return self + 0x18 + index * 384;
}

int main()
{
    const int SZ = 0x1000;
    char* n = new char[SZ];
    std::memset(n, 0, SZ);

    // flag set -> NULL
    n[0x33c] = 1;
    *reinterpret_cast<int*>(n + 0x318) = 3;
    if (model(n) != 0) { std::printf("BAD1\n"); return 1; }

    // flag clear, index 0 -> base + 0x18
    n[0x33c] = 0;
    *reinterpret_cast<int*>(n + 0x318) = 0;
    if (model(n) != n + 0x18) { std::printf("BAD2\n"); return 1; }

    // index 2 -> base + 0x18 + 2*384
    *reinterpret_cast<int*>(n + 0x318) = 2;
    if (model(n) != n + 0x18 + 2 * 384) { std::printf("BAD3\n"); return 1; }

    std::printf("GETREGION_OK\n");
    delete[] n;
    return 0;
}