#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fda4a
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fda4a::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fda4a* FABLE_FASTCALL
FableGetActionName_008fda4a(FableCharStringResult_008fda4a* self, void*);

int main()
{
    FableCharStringResult_008fda4a r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fda4a* got = FableGetActionName_008fda4a(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fda4a_OK\n");
    return 0;
}