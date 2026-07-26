struct CBase
{
    CBase();
    virtual void VTableAnchor();
};

struct CSurface
{
    void* resource04;
    unsigned long resourceState08;
    unsigned long unknown0C;

    __forceinline CSurface()
        : resource04(0),
          resourceState08(0)
    {
    }

    virtual void VTableAnchor();
    CSurface& operator=(const CSurface& source);
};

struct CRenderTarget : CBase
{
    CSurface primary04;
    CSurface secondary14;

    CRenderTarget(
        const CSurface& primary,
        const CSurface& secondary);

    virtual void VTableAnchor();
};

CRenderTarget::CRenderTarget(
    const CSurface& primary,
    const CSurface& secondary)
{
    primary04 = primary;
    secondary14 = secondary;
}
