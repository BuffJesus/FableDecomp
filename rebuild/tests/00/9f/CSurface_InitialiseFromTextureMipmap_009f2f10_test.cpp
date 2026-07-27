#include <stdio.h>

struct IDirect3DSurface9
{
    virtual void __stdcall QueryInterfacePlaceholder() = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

struct CSurface
{
    void* vtable00;
    IDirect3DSurface9* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);
};

struct TestSurface : IDirect3DSurface9
{
    unsigned long addRefCalls;
    unsigned long releaseCalls;

    TestSurface() : addRefCalls(0), releaseCalls(0)
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
        return ++releaseCalls;
    }
};

int main()
{
    TestSurface first;
    TestSurface second;
    CSurface wrapper = {};

    wrapper.InitialiseFromTextureMipmap(&first);
    if (
        wrapper.surface04 != &first ||
        wrapper.ownership08 != 3 ||
        first.addRefCalls != 0 ||
        first.releaseCalls != 0)
    {
        printf("FABLETLC_SURFACE_INITIALISE_TEXTURE_MIPMAP_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    wrapper.InitialiseFromTextureMipmap(&second);
    if (
        wrapper.surface04 != &second ||
        wrapper.ownership08 != 3 ||
        first.releaseCalls != 1 ||
        second.addRefCalls != 0)
    {
        printf("FABLETLC_SURFACE_INITIALISE_TEXTURE_MIPMAP_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_SURFACE_INITIALISE_TEXTURE_MIPMAP_BEHAVIOR PASS\n");
    return 0;
}
