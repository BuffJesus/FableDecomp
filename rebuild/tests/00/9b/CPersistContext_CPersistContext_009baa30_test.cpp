#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/9b/CPersistContext_CPersistContext_009baa30.cpp"

struct TestPersistContext_009baa30
{
    unsigned long currentSection;
    unsigned char untouched04[0x11];
    unsigned char flag15;
    unsigned char untouched16[2];
    unsigned long mode;
    unsigned char untouched1c[8];
    void* inputStream;
    void* outputStream;
};

static int CheckConstruction(bool option)
{
    TestPersistContext_009baa30 context;
    std::memset(&context, 0xA5, sizeof(context));
    unsigned long inputMarker = 0x13572468;

    typedef void* (__fastcall *ConstructFunction)(
        void*,
        void*,
        void*,
        bool);
    ConstructFunction volatile construct =
        &CPersistContext_CPersistContext_009baa30;

    void* result = construct(&context, 0, &inputMarker, option);
    if (context.currentSection != 0 ||
        context.flag15 != 0 ||
        context.mode != 2 ||
        context.inputStream != &inputMarker ||
        context.outputStream != 0)
    {
        return 1;
    }

    // Retail leaves the surrounding state alone and does not establish an
    // explicit return value.
    for (int index = 0; index < 0x11; ++index)
    {
        if (context.untouched04[index] != 0xA5)
        {
            return 1;
        }
    }
    if (context.untouched16[0] != 0xA5 ||
        context.untouched16[1] != 0xA5)
    {
        return 1;
    }
    for (int index = 0; index < 8; ++index)
    {
        if (context.untouched1c[index] != 0xA5)
        {
            return 1;
        }
    }

    (void)result;
    return 0;
}

int main()
{
    int failures = 0;
    failures += CheckConstruction(false);
    failures += CheckConstruction(true);

    if (failures != 0)
    {
        std::printf(
            "PERSIST_CONTEXT_CTOR_009baa30_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("PERSIST_CONTEXT_CTOR_009baa30_TEST PASS\n");
    return 0;
}
