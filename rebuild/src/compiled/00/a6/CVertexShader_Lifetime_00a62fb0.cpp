#include "fable_vertex_shader.h"
#include "fable_d3d9_shader_create.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
CVertexShader::CVertexShader():Data(0) {}
unsigned long CVertexShader::CountReferences(){return Data->ReferenceCount;}
CVertexShaderData::CVertexShaderData(IDirect3DVertexDeclaration9* declaration,IDirect3DVertexShader9* shader,const D3DVERTEXELEMENT9* format,const CCharString&,const CCharString&)
    : VertexDeclaration(declaration),VertexShader(shader),FormatDecl(format),ReferenceCount(1) {}
CVertexShaderData::~CVertexShaderData(){
    if(VertexShader) VertexShader->Release();
    if(VertexDeclaration) VertexDeclaration->Release();
}

CVertexShader::CVertexShader(const CArrayVector<unsigned long>& code,const D3DVERTEXELEMENT9* elements,const CCharString& name,const CCharString& source)
{
    IDirect3DDevice9* device=GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    IDirect3DVertexShader9* shader;
    IDirect3DVertexDeclaration9* declaration;
    FableCreateVertexShader(device,code.GetAsCArray(),&shader);
    FableCreateVertexDeclaration(device,elements,&declaration);
    Data=new CVertexShaderData(declaration,shader,elements,name,source);
}
