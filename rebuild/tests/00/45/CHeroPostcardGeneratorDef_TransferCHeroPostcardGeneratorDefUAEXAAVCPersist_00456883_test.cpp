#include <cstdio>
struct CPersistContext {
    int calls; void* last;
    void PutA(void* p){ calls++; last=p; }
    void PutB(void* p){ calls+=10; last=p; }
    void PutC(void* p){ calls+=100; last=p; }
};
struct CHeroPostcardGeneratorDef {
    char pad[0x58];
    char base[4];
};

void Transfer_impl(CHeroPostcardGeneratorDef* self, CPersistContext* ctx)
{
    ctx->PutA((char*)self + 0x39);
    ctx->PutA((char*)self + 0x41);
    ctx->PutB((char*)self + 0x49);
    ctx->PutC((char*)self + 0x58);
}

int main(){
    CHeroPostcardGeneratorDef d;
    CPersistContext c; c.calls=0; c.last=0;
    Transfer_impl(&d, &c);
    if (c.calls==112 && c.last==(char*)&d+0x58){ std::printf("CHeroPostcardGeneratorDef_00456883_TEST PASS\n"); return 0; }
    std::printf("FAIL calls=%d\n", c.calls); return 1;
}