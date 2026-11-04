#pragma once
#include "fable_base_class.h"
#include "fable_d3d9_shader.h"
#include "fable_string.h"
#include "fable_carray.h"
class CVertexShader;
class CShaderBase {
    friend class CShaderResource;
protected:
    CShaderBase() {}
    virtual ~CShaderBase() {}
    virtual CCharString GetDebugName() const;
private:
    virtual unsigned long CountReferences()=0;
};
class CVertexShaderData : public CBaseClassNonCopyable {
    friend class CShaderRenderManager;
    friend class CVertexShader;
    friend struct CVertexShaderTestAccess;
    CVertexShaderData(IDirect3DVertexDeclaration9*,IDirect3DVertexShader9*,const D3DVERTEXELEMENT9*,const CCharString&,const CCharString&);
    virtual ~CVertexShaderData();
    IDirect3DVertexDeclaration9* VertexDeclaration;
    IDirect3DVertexShader9* VertexShader;
    const D3DVERTEXELEMENT9* FormatDecl;
    unsigned long ReferenceCount;
};
class CVertexShader : public CShaderBase {
    friend class CShaderRenderManager;
    friend class CShaderResource;
    friend struct CVertexShaderTestAccess;
public:
    CVertexShader();
    CVertexShader(const CVertexShader& other) : Data(other.Data) {
        if (Data) ++Data->ReferenceCount;
    }
    const CVertexShader& operator=(const CVertexShader& other) {
        CVertexShaderData* previous = Data;
        if (previous && --previous->ReferenceCount == 0) delete previous;
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual __forceinline ~CVertexShader() {
        CVertexShaderData* data=Data;
        if(data && --data->ReferenceCount==0) delete data;
    }
private:
    CVertexShader(const CArrayVector<unsigned long>&,const D3DVERTEXELEMENT9*,const CCharString&,const CCharString&);
    virtual unsigned long CountReferences();
    CVertexShaderData* Data;
};
FABLE_STATIC_ASSERT(sizeof(CShaderBase)==4);
FABLE_STATIC_ASSERT(sizeof(CVertexShader)==8);
FABLE_STATIC_ASSERT(sizeof(CVertexShaderData)==20);
