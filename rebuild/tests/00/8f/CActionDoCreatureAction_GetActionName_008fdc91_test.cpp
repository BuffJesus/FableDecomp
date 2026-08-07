#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fdc91
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fdc91::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fdc91* FABLE_FASTCALL
FableGetActionName_008fdc91(FableCharStringResult_008fdc91* self, void*);

int main()
{
    FableCharStringResult_008fdc91 r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fdc91* got = FableGetActionName_008fdc91(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fdc91_OK\n");
    return 0;
}