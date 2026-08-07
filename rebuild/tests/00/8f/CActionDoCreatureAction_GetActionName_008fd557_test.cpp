#include "rebuild_abi.h"
#include <cstdio>
struct FableCharStringResult_008fd557
{ const char* seen_text; int seen_len; void* build(const char* text, int length); };
void* FableCharStringResult_008fd557::build(const char* text, int length)
{ seen_text = text; seen_len = length; return this; }
extern "C" FableCharStringResult_008fd557* FABLE_FASTCALL
FableGetActionName_008fd557(FableCharStringResult_008fd557* self, void*);
int main()
{
    FableCharStringResult_008fd557 r; r.seen_text = 0; r.seen_len = 0;
    FableCharStringResult_008fd557* got = FableGetActionName_008fd557(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fd557_OK\n"); return 0;
}