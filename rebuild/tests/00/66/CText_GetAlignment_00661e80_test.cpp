#include "rebuild_abi.h"
#include <cstdio>
struct FableAlign_00661e80
{
    char pad[0x15c];
    unsigned char flags;
    char FABLE_FASTCALL get();
};
extern char FABLE_FASTCALL FableAlign_get_00661e80(FableAlign_00661e80* p);
int main()
{
    FableAlign_00661e80 a;
    a.flags = 0x04; if (a.get() != 1) { std::printf("BAD1\n"); return 1; }
    a.flags = 0x08; if (a.get() != 1) { std::printf("BAD2\n"); return 1; }
    a.flags = 0x00; if (a.get() != 0) { std::printf("BAD3\n"); return 1; }
    a.flags = 0x0c; if (a.get() != 1) { std::printf("BAD4\n"); return 1; }
    std::printf("GETALIGN_00661e80_OK\n");
    return 0;
}