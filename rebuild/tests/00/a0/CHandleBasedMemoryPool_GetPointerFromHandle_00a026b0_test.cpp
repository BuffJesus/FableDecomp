#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FablePool_00a026b0 { void* GetPointerFromHandle(int handle); };
void* FablePool_00a026b0::GetPointerFromHandle(int handle)
{
    char** table = *(char***)((char*)this + 0x438);
    return table[handle] + 0x20;
}
int main() {
    unsigned char buf[0x440];
    std::memset(buf, 0, sizeof(buf));
    char* slots[4];
    char block[0x40];
    slots[2] = block;
    *(char***)(buf + 0x438) = slots;
    void* p = ((FablePool_00a026b0*)buf)->GetPointerFromHandle(2);
    if (p != block + 0x20) { std::printf("BAD_PTR\n"); return 1; }
    std::printf("GPFH_00a026b0_OK\n"); return 0;
}