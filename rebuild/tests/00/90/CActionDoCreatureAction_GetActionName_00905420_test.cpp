#include "rebuild_abi.h"
#include <cstdio>

struct CCharString {
    int dummy;
    const char* seen_lit;
    int seen_len;
    CCharString* ctor_99ebf0(const char* lit, int len);
};

CCharString* CCharString::ctor_99ebf0(const char* lit, int len)
{
    seen_lit = lit;
    seen_len = len;
    dummy = 0x1234;
    return this;
}

extern "C" CCharString* FABLE_FASTCALL
FableGetActionName_00905420(void* thisptr, void* edx, CCharString* result);

int main()
{
    CCharString r;
    r.dummy = 0;
    r.seen_lit = 0;
    r.seen_len = 0;
    CCharString* got = FableGetActionName_00905420((void*)0xDEAD, (void*)0, &r);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_lit == 0) { std::printf("BAD_TEXT\n"); return 1; }
    if (r.dummy != 0x1234) { std::printf("BAD_CTOR\n"); return 1; }
    std::printf("GETACTIONNAME_00905420_OK\n");
    return 0;
}