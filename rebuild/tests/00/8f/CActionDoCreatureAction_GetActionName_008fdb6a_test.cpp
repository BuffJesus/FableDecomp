#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fdb6a
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fdb6a::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fdb6a* FABLE_FASTCALL
FableGetActionName_008fdb6a(FableCharStringResult_008fdb6a* self, void*);

int main()
{
    FableCharStringResult_008fdb6a r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fdb6a* got = FableGetActionName_008fdb6a(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fdb6a_OK\n");
    return 0;
}