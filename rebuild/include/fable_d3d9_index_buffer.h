#pragma once
// IDirect3DIndexBuffer9 (d3d9.h vtable order: IUnknown, IDirect3DResource9, then Lock, Unlock, GetDesc) and
// D3DINDEXBUFFER_DESC, as CIndexBuffer uses them.
#include "fable_d3d9.h"

typedef struct _D3DINDEXBUFFER_DESC
{
    D3DFORMAT Format;
    DWORD Type;     // D3DRESOURCETYPE
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
} D3DINDEXBUFFER_DESC;

struct IDirect3DIndexBuffer9 : public IDirect3DResource9
{
    virtual HRESULT __stdcall Lock(UINT offset, UINT size, void** data, DWORD flags) = 0;
    virtual HRESULT __stdcall Unlock() = 0;
    virtual HRESULT __stdcall GetDesc(D3DINDEXBUFFER_DESC* desc) = 0;
};
