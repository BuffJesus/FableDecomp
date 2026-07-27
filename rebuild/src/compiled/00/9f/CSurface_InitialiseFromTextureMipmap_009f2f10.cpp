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

void CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9* surface)
{
    if (surface04 != 0)
    {
        surface04->Release();
        surface04 = 0;
        ownership08 = 0;
    }

    surface04 = surface;
    ownership08 = 3;
}
