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

void CSurface::Uninitialise()
{
    if (resource04 != 0)
    {
        resource04->Release();
        resource04 = 0;
        resourceState08 = 0;
    }
}
