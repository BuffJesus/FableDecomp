#include "rebuild_abi.h"
#include <cstdio>

// Behaviour fixture for CActionDoCreatureAction::GetActionName @ 0x004d4f6e.
// The tested function forwards a relocated string constant plus the -1 length
// sentinel to the CCharString builder member and returns the result pointer
// (this) unchanged. We supply a stand-in builder that records the arguments,
// proving the forwarded sentinel and the returned identity.
struct FableCharStringResult_004d4f6e
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

// Out-of-line so the definition is emitted as a real symbol that resolves the
// builder reference inside the tested function's object.
void* FableCharStringResult_004d4f6e::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_004d4f6e* FABLE_FASTCALL
FableGetActionName_004d4f6e(FableCharStringResult_004d4f6e* self, void*);

int main()
{
    FableCharStringResult_004d4f6e r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_004d4f6e* got = FableGetActionName_004d4f6e(&r, 0);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text == 0) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("GETACTIONNAME_004d4f6e_OK\n");
    return 0;
}