#include "rebuild_abi.h"
#include <cstdio>
struct FableCharStringResult_008fd31d
{ const char* seen_text; int seen_len; void* build(const char* text, int length); };
void* FableCharStringResult_008fd31d::build(const char* text, int length)
{ seen_text = text; seen_len = length; return this; }
extern "C" FableCharStringResult_008fd31d* FABLE_FASTCALL
FableGetActionName_008fd31d(FableCharStringResult_008fd31d* self, void*);
int main()
{
    FableCharStringResult_008fd31d r; r.seen_text = 0; r.seen_len = 0;
    FableCharStringResult_008fd31d* got = FableGetActionName_008fd31d(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_008fd31d_OK\n"); return 0;
}