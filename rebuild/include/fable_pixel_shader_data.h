#pragma once
#include "fable_base_class.h"

struct IDirect3DPixelShader9;

// Ego_r CPixelShaderData; retail DisablePixelShaders 00988190 confirms
// the deleting-destructor slot and reference count at +8.
class CPixelShaderData : public CBaseClassNonCopyable
{
    friend class CShaderRenderManager;
    friend class CPixelShader;
public:
    virtual ~CPixelShaderData();
    void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
private:
    IDirect3DPixelShader9* PixelShader;
    unsigned long ReferenceCount;
    unsigned long ConstCount;
    long* ConstIndices;
};
FABLE_STATIC_ASSERT(sizeof(CPixelShaderData)==0x14);
