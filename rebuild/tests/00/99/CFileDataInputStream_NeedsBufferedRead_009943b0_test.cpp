#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct FableStream_009943b0 { bool NeedsBufferedRead(int requested); };
bool FableStream_009943b0::NeedsBufferedRead(int requested)
{
    int threshold = *(const int*)((const char*)this + 0x20);
    return requested < threshold;
}
int main() {
    unsigned char buf[0x40];
    std::memset(buf, 0, sizeof(buf));
    *(int*)(buf + 0x20) = 100;
    FableStream_009943b0* s = (FableStream_009943b0*)buf;
    if (s->NeedsBufferedRead(50) != true) { std::printf("BAD_LT\n"); return 1; }
    if (s->NeedsBufferedRead(100) != false) { std::printf("BAD_EQ\n"); return 1; }
    if (s->NeedsBufferedRead(200) != false) { std::printf("BAD_GT\n"); return 1; }
    std::printf("NBR_009943b0_OK\n"); return 0;
}