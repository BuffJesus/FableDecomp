#pragma once
#include "fable_d3d9_shader.h"
#include "rebuild_abi.h"
#include <stddef.h>

// Sparse, typed view of the two device COM slots used by shader creation.
// Windows SDK d3d9.h confirms signatures; retail A63020 confirms offsets.
struct FableD3D9ShaderCreationVTable
{
    void* BeforeCreateVertexDeclaration[86];
    HRESULT (__stdcall *CreateVertexDeclaration)(IDirect3DDevice9*,const D3DVERTEXELEMENT9*,IDirect3DVertexDeclaration9**);
    void* BetweenCreationMethods[4];
    HRESULT (__stdcall *CreateVertexShader)(IDirect3DDevice9*,const DWORD*,IDirect3DVertexShader9**);
};
FABLE_STATIC_ASSERT(offsetof(FableD3D9ShaderCreationVTable,CreateVertexDeclaration)==0x158);
FABLE_STATIC_ASSERT(offsetof(FableD3D9ShaderCreationVTable,CreateVertexShader)==0x16c);
inline HRESULT FableCreateVertexShader(IDirect3DDevice9* device,const DWORD* code,IDirect3DVertexShader9** shader)
{
    const FableD3D9ShaderCreationVTable* table=*reinterpret_cast<const FableD3D9ShaderCreationVTable* const*>(device);
    return table->CreateVertexShader(device,code,shader);
}
inline HRESULT FableCreateVertexDeclaration(IDirect3DDevice9* device,const D3DVERTEXELEMENT9* elements,IDirect3DVertexDeclaration9** declaration)
{
    const FableD3D9ShaderCreationVTable* table=*reinterpret_cast<const FableD3D9ShaderCreationVTable* const*>(device);
    return table->CreateVertexDeclaration(device,elements,declaration);
}
