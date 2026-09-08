#include "engine/CSurface.h"
typedef long HRESULT;
struct D3DSURFACE_DESC_RETAIL {
    unsigned long Format, Type, Usage, Pool, MultiSampleType, MultiSampleQuality;
    unsigned long Width, Height;
};
struct IDirect3DSurface9Retail {
    virtual HRESULT __stdcall QueryInterface() = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual HRESULT __stdcall GetDevice() = 0;
    virtual HRESULT __stdcall SetPrivateData() = 0;
    virtual HRESULT __stdcall GetPrivateData() = 0;
    virtual HRESULT __stdcall FreePrivateData() = 0;
    virtual unsigned long __stdcall SetPriority() = 0;
    virtual unsigned long __stdcall GetPriority() = 0;
    virtual void __stdcall PreLoad() = 0;
    virtual unsigned long __stdcall GetType() = 0;
    virtual HRESULT __stdcall GetContainer() = 0;
    virtual HRESULT __stdcall GetDesc(D3DSURFACE_DESC_RETAIL* description) = 0;
};
struct C2DExtentsI { long X; long Y; };
struct CSurfaceMethods : CSurface {
    C2DExtentsI GetDimensions() const;
};
C2DExtentsI CSurfaceMethods::GetDimensions() const
{
    D3DSURFACE_DESC_RETAIL description;
    reinterpret_cast<IDirect3DSurface9Retail*>(PD3DSurface)->GetDesc(&description);
    C2DExtentsI dimensions = { (long)description.Width, (long)description.Height };
    return dimensions;
}