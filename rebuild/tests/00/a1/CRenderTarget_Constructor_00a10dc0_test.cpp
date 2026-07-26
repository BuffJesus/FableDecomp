#include <stdio.h>

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

CBase::CBase()
{
}

void CBase::VTableAnchor()
{
}

void CSurface::VTableAnchor()
{
}

CSurface& CSurface::operator=(const CSurface& source)
{
    resource04 = source.resource04;
    resourceState08 = source.resourceState08;
    return *this;
}

void CRenderTarget::VTableAnchor()
{
}

int main()
{
    int primaryResource;
    int secondaryResource;
    CSurface primary;
    CSurface secondary;
    primary.resource04 = &primaryResource;
    primary.resourceState08 = 11;
    secondary.resource04 = &secondaryResource;
    secondary.resourceState08 = 22;

    CRenderTarget target(primary, secondary);
    if (target.primary04.resource04 != &primaryResource ||
        target.primary04.resourceState08 != 11)
    {
        printf("FABLETLC_RENDER_TARGET_CONSTRUCTOR_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (target.secondary14.resource04 != &secondaryResource ||
        target.secondary14.resourceState08 != 22)
    {
        printf("FABLETLC_RENDER_TARGET_CONSTRUCTOR_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_RENDER_TARGET_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
