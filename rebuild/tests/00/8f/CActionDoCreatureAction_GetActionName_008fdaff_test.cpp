#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fdaff
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fdaff::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fdaff* FABLE_FASTCALL
FableGetActionName_008fdaff(FableCharStringResult_008fdaff* self, void*);

int main()
{
    FableCharStringResult_008fdaff r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fdaff* got = FableGetActionName_008fdaff(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fdaff_OK\n");
    return 0;
}