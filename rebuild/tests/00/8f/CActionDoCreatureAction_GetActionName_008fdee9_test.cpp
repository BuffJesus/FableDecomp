#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fdee9
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fdee9::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fdee9* FABLE_FASTCALL
FableGetActionName_008fdee9(FableCharStringResult_008fdee9* self, void*);

int main()
{
    FableCharStringResult_008fdee9 r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fdee9* got = FableGetActionName_008fdee9(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fdee9_OK\n");
    return 0;
}