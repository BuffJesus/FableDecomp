#include <stdio.h>

struct CSurfaceResource
{
    virtual void __stdcall QueryInterfacePlaceholder() = 0;
    virtual void __stdcall AddRefPlaceholder() = 0;
    virtual void __stdcall Release() = 0;
};

struct CSurface
{
    void* vtable00;
    CSurfaceResource* resource04;
    unsigned long resourceState08;

    void Uninitialise();
};

struct TestSurfaceResource : CSurfaceResource
{
    int releaseCalls;

    TestSurfaceResource() : releaseCalls(0)
    {
    }

    virtual void __stdcall QueryInterfacePlaceholder()
    {
    }

    virtual void __stdcall AddRefPlaceholder()
    {
    }

    virtual void __stdcall Release()
    {
        ++releaseCalls;
    }
};

int main()
{
    CSurface empty = {};
    empty.resourceState08 = 0x12345678;
    empty.Uninitialise();
    if (empty.resourceState08 != 0x12345678)
    {
        printf("FABLETLC_SURFACE_UNINITIALISE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    TestSurfaceResource resource;
    CSurface populated = {};
    populated.resource04 = &resource;
    populated.resourceState08 = 0x12345678;
    populated.Uninitialise();
    if (resource.releaseCalls != 1)
    {
        printf("FABLETLC_SURFACE_UNINITIALISE_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    if (populated.resource04 != 0 || populated.resourceState08 != 0)
    {
        printf("FABLETLC_SURFACE_UNINITIALISE_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_SURFACE_UNINITIALISE_BEHAVIOR PASS\n");
    return 0;
}
