#include <cstdio>

struct CObservable {
    char pad[0x134];
    void* exclusiveObserver;
};

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* edx_unused, void* observer)
{
    self->exclusiveObserver = observer;
}

int main()
{
    CObservable o;
    o.exclusiveObserver = 0;
    int dummy = 42;
    CObservable_AddExclusiveObserver(&o, 0, &dummy);
    if (o.exclusiveObserver == &dummy) {
        std::printf("CObservable_0041ca31_TEST PASS\n");
        return 0;
    }
    std::printf("CObservable_0041ca31_TEST FAIL\n");
    return 1;
}