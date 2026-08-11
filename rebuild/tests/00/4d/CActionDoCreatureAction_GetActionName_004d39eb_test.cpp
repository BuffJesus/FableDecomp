#include <cstdio>

struct FableCharStringResult_004d39eb
{
    const char* seen_text;
    int seen_len;
    void* build(const char* text, int length);
};

static FableCharStringResult_004d39eb* g_last;
void* FableCharStringResult_004d39eb::build(const char* text, int length)
{
    seen_text = text;
    seen_len = length;
    g_last = this;
    return this;
}

extern "C" FableCharStringResult_004d39eb* __fastcall
FableGetActionName_004d39eb(FableCharStringResult_004d39eb* self, void*);

int main()
{
    FableCharStringResult_004d39eb obj;
    obj.seen_text = 0;
    obj.seen_len = 0;
    FableCharStringResult_004d39eb* ret = FableGetActionName_004d39eb(&obj, 0);

    bool ok = (ret == &obj)
        && (obj.seen_text == (const char*)19115712)
        && (obj.seen_len == -1);

    if (ok)
        printf("004d39eb_TEST PASS\n");
    else
        printf("FAIL text=%p len=%d ret=%p\n", obj.seen_text, obj.seen_len, ret);
    return 0;
}