#include <new>
#include <stdio.h>
#include <string.h>

struct IUnknownLike
{
    virtual void __stdcall QueryInterfacePlaceholder() = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

class CSurface
{
public:
    virtual void VTableAnchor();

    IUnknownLike* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    CSurface(const CSurface& source);
};

struct TestUnknown : IUnknownLike
{
    unsigned long addRefCalls;

    TestUnknown() : addRefCalls(0)
    {
    }

    virtual void __stdcall QueryInterfacePlaceholder()
    {
    }

    virtual unsigned long __stdcall AddRef()
    {
        return ++addRefCalls;
    }

    virtual unsigned long __stdcall Release()
    {
        return 0;
    }
};

void CSurface::VTableAnchor()
{
}

int main()
{
    TestUnknown unknown;
    unsigned char sourceStorage[sizeof(CSurface)];
    memset(sourceStorage, 0, sizeof(sourceStorage));
    CSurface& source = *reinterpret_cast<CSurface*>(sourceStorage);
    source.surface04 = &unknown;
    source.ownership08 = 2;
    source.mipLevel0C = 7;

    unsigned char storage[sizeof(CSurface)];
    CSurface* copy = new (storage) CSurface(source);
    if (
        copy->surface04 != &unknown ||
        copy->ownership08 != 2 ||
        copy->mipLevel0C != 7 ||
        unknown.addRefCalls != 1)
    {
        printf("FABLETLC_SURFACE_COPY_CONSTRUCTOR_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    source.ownership08 = 3;
    source.mipLevel0C = 11;
    copy->mipLevel0C = 0xA5A5A5A5;
    copy = new (storage) CSurface(source);
    if (
        copy->ownership08 != 3 ||
        copy->mipLevel0C != 0xA5A5A5A5 ||
        unknown.addRefCalls != 2)
    {
        printf("FABLETLC_SURFACE_COPY_CONSTRUCTOR_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_SURFACE_COPY_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
