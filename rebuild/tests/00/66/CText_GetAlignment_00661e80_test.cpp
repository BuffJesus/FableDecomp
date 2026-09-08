#include "engine/CText.h"
#include "rebuild_abi.h"
#include <cstdio>

struct CText_Methods : CText {
    char FABLE_FASTCALL get();
};

int main()
{
    CText_Methods a;
    a.flags = 0x04; if (a.get() != 1) { std::printf("BAD1\n"); return 1; }
    a.flags = 0x08; if (a.get() != 1) { std::printf("BAD2\n"); return 1; }
    a.flags = 0x00; if (a.get() != 0) { std::printf("BAD3\n"); return 1; }
    a.flags = 0x0c; if (a.get() != 1) { std::printf("BAD4\n"); return 1; }
    std::printf("GETALIGN_00661e80_OK\n");
    return 0;
}
