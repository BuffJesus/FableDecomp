#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fe05c
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fe05c::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fe05c* FABLE_FASTCALL
FableGetActionName_008fe05c(FableCharStringResult_008fe05c* self, void*);

int main()
{
    FableCharStringResult_008fe05c r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fe05c* got = FableGetActionName_008fe05c(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fe05c_OK\n");
    return 0;
}