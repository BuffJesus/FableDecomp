#include <cstdio>
struct Inner { char pad[0x14]; unsigned long base; };
struct CAIStateGroupBase { char pad0[4]; Inner* p; };

unsigned long* __fastcall CAIStateGroupBase_GetPWorldSeed(const CAIStateGroupBase* self)
{
    unsigned long v = self->p->base;
    return (unsigned long*)(v + 0x1613c);
}

int main(){
    Inner inner; inner.base = 0x1000;
    CAIStateGroupBase obj; obj.p = &inner;
    unsigned long* r = CAIStateGroupBase_GetPWorldSeed(&obj);
    if ((unsigned long)r == 0x1000UL + 0x1613cUL) {
        std::printf("CAIStateGroupBase_008fcfd0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %p\n", (void*)r);
    return 1;
}