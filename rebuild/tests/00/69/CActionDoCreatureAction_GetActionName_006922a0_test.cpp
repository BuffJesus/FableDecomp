#include <cstdio>

struct FableCharStringResult_006922a0
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

void* FableCharStringResult_006922a0::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    return this;
}

extern "C" FableCharStringResult_006922a0* __stdcall
FableGetActionName_006922a0(FableCharStringResult_006922a0* self);

int main()
{
    FableCharStringResult_006922a0 r;
    r.seen_text = 0;
    r.seen_len = 0;
    FableCharStringResult_006922a0* got = FableGetActionName_006922a0(&r);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (r.seen_text != (const char*)0x0125c18c) { std::printf("BAD_TEXT\n"); return 1; }
    std::printf("006922a0_TEST PASS\n");
    return 0;
}