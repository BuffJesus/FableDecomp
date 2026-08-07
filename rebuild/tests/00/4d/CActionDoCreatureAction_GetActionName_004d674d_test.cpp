#include "rebuild_abi.h"
#include <cstdio>
struct FableCharStringResult_004d674d
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};
void* FableCharStringResult_004d674d::build(const char* text, int length)
{
    seen_text = text; seen_len = length; return this;
}
extern "C" FableCharStringResult_004d674d* FABLE_FASTCALL
FableGetActionName_004d674d(FableCharStringResult_004d674d* self, void*);
int main()
{
    FableCharStringResult_004d674d r; r.seen_text = 0; r.seen_len = 0;
    FableCharStringResult_004d674d* got = FableGetActionName_004d674d(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_004d674d_OK\n");
    return 0;
}