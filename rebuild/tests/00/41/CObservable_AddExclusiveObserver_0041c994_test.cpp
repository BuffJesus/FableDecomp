#include <cstdio>

struct CObservable { char buf[0x140]; };

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* /*edx*/, void* observer)
{
    *(void**)((char*)self + 0x13c) = observer;
}

int main()
{
    CObservable o;
    for (int i = 0; i < (int)sizeof(o.buf); ++i) o.buf[i] = 0;
    int marker = 0x1234;
    CObservable_AddExclusiveObserver(&o, 0, &marker);
    void* got = *(void**)((char*)&o + 0x13c);
    if (got == &marker) {
        std::printf("CObservable_0041c994_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}