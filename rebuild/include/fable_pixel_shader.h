#pragma once
#include "fable_vertex_shader.h"
#include "fable_pixel_shader_data.h"
class CPixelShader : public CShaderBase {
    friend class CShaderRenderManager;
public:
    CPixelShader();
    virtual __forceinline ~CPixelShader(){if(Data)Data->Release();}
private:
    virtual unsigned long CountReferences();
    CPixelShaderData* Data;
};
FABLE_STATIC_ASSERT(sizeof(CPixelShader)==8);
