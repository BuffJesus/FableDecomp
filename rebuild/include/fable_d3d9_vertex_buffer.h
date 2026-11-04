#pragma once
#include "fable_d3d9.h"
struct _D3DVERTEXBUFFER_DESC;
typedef struct _D3DVERTEXBUFFER_DESC D3DVERTEXBUFFER_DESC;
struct IDirect3DVertexBuffer9 : public IDirect3DResource9 {
    virtual HRESULT __stdcall Lock(UINT offset,UINT size,void** data,DWORD flags)=0;
    virtual HRESULT __stdcall Unlock()=0;
    virtual HRESULT __stdcall GetDesc(D3DVERTEXBUFFER_DESC*)=0;
};
