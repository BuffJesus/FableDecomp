#include "rebuild_abi.h"
#include <cstdio>

struct FableCharStringResult_008fdfba
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_008fdfba::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_008fdfba* FABLE_FASTCALL
FableGetActionName_008fdfba(FableCharStringResult_008fdfba* self, void*);

int main()
{
    FableCharStringResult_008fdfba r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_008fdfba* got = FableGetActionName_008fdfba(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fdfba_OK\n");
    return 0;
}