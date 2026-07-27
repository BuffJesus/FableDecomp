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

CSurface::CSurface(const CSurface& source)
    : surface04(source.surface04),
      ownership08(source.ownership08)
{
    if (ownership08 == 2)
    {
        mipLevel0C = source.mipLevel0C;
    }

    if (surface04 != 0)
    {
        surface04->AddRef();
    }
}
