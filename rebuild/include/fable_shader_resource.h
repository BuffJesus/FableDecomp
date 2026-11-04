#pragma once
#include "fable_vertex_shader.h"
#include "fable_vertex_shader_input.h"
#include "fable_font_vertex_buffer.h"
class CShaderDataBank;
class CShaderRenderManager;
class CShaderResource : public CBaseClassNonCopyable
{
    friend class CShaderRenderManager;
    friend struct CShaderResourceTestAccess;
public:
    virtual ~CShaderResource();
    void CloseShaderBank();
    void OpenShaderBank(const CShaderDataBank*);
    void SetVertexShaderDeclaration(const D3DVERTEXELEMENT9*);
    void SetVertexFormat(EVertexType);
    void SetVertexShaderInput(const CVertexShaderInput&);
    const CShaderDataBank* PeekShaderBank() const;
    CVertexShader GetVertexShader(const CCharString& name) const;
    CVertexShader GetVertexShader(unsigned long index) const;
private:
    CShaderResource();
    void Uninitialise();
    void Initialise(CShaderRenderManager&);
    CShaderDataBank* ShaderBank;
    mutable _STL::vector<CShaderBase*> Shaders;
    CShaderRenderManager* ShaderRenderManager;
    CVertexShaderInput VertexShaderInput;
    D3DVERTEXELEMENT9* VertexShaderDeclaration;
    EVertexType VertexFormat;
};
FABLE_STATIC_ASSERT(sizeof(CShaderResource)==0x3c);
