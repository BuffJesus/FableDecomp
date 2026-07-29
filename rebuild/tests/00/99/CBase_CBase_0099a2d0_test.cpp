#include <cstdio>

#include "../../../src/compiled/00/99/CBase_CBase_0099a2d0.cpp"

extern "C" void* CBase_VTable_0099a2d0[1] = {
    (void*)0x13572468
};

struct TestBase_0099a2d0
{
    void* vtable;
    unsigned long derivedSentinel;
};

int main()
{
    TestBase_0099a2d0 object;
    object.vtable = (void*)0xaaaaaaaa;
    object.derivedSentinel = 0x55aa55aa;

    typedef void* (__fastcall *ConstructorFunction)(void*, void*);
    ConstructorFunction volatile construct = &CBase_CBase_0099a2d0;
    void* result = construct(&object, 0);

    if (result != &object ||
        object.vtable != CBase_VTable_0099a2d0 ||
        object.derivedSentinel != 0x55aa55aa)
    {
        std::printf(
            "CBASE_0099a2d0_TEST FAIL result=%p vtable=%p sentinel=%08lx\n",
            result,
            object.vtable,
            object.derivedSentinel);
        return 1;
    }

    std::printf("CBASE_0099a2d0_TEST PASS\n");
    return 0;
}
