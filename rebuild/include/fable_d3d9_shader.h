#pragma once
#include "fable_d3d9.h"
#if !defined(_WINDOWS_) && !defined(_D3D9_H_)
struct _D3DVERTEXELEMENT9 {
    unsigned short Stream,Offset;
    unsigned char Type,Method,Usage,UsageIndex;
};
typedef _D3DVERTEXELEMENT9 D3DVERTEXELEMENT9;
struct IDirect3DVertexShader9 : public IUnknown {
    virtual HRESULT __stdcall GetDevice(IDirect3DDevice9** device) = 0;
    virtual HRESULT __stdcall GetFunction(void* data,UINT* size) = 0;
};
struct IDirect3DVertexDeclaration9 : public IUnknown {
    virtual HRESULT __stdcall GetDevice(IDirect3DDevice9** device) = 0;
    virtual HRESULT __stdcall GetDeclaration(D3DVERTEXELEMENT9* elements,UINT* count) = 0;
};

#endif
