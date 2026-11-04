#include <new>
#include "fable_shader_render_manager.h"
#include "fable_shader_resource.h"
#include "fable_indexed_array.h"
#include "fable_shader_constant_layout.h"
namespace _STL {
 template<> C4DVector* vector<C4DVector>::erase(C4DVector*,C4DVector*);
 template<> void vector<C4DVector>::_M_fill_insert(C4DVector*,size_t,const C4DVector&);
 template<> void vector<const CVertexShaderConstantLayout*>::resize(size_t,const CVertexShaderConstantLayout* const&);
}
#include "fable_stlport_vector_impl.h"
namespace {
__forceinline void Identity(CMatrix4x4& m) {
    m.E11=1.0f;
    m.E22=1.0f;
    m.E33=1.0f;
    m.E44=1.0f;
    m.E12=0.0f;
    m.E13=0.0f;
    m.E14=0.0f;
    m.E21=0.0f;
    m.E23=0.0f;
    m.E24=0.0f;
    m.E31=0.0f;
    m.E32=0.0f;
    m.E34=0.0f;
    m.E41=0.0f;
    m.E42=0.0f;
    m.E43=0.0f;
}
__forceinline void WorldIdentity(CMatrix4x4& m) {
    m.E12=0.0f;
    m.E13=0.0f;
    m.E14=0.0f;
    m.E21=0.0f;
    m.E23=0.0f;
    m.E24=0.0f;
    m.E31=0.0f;
    m.E32=0.0f;
    m.E34=0.0f;
    m.E41=0.0f;
    m.E42=0.0f;
    m.E43=0.0f;
    m.E11=1.0f;
    m.E22=1.0f;
    m.E33=1.0f;
    m.E44=1.0f;
}
}
void CShaderRenderManager::Initialise(CRenderManager* render,IDirect3DDevice9* device) {
    D3DDevice=0;CurrentVertexShader=0;CurrentVertexShaderVertexDeclairation=0;
    SWLightingEnable=false;VertexShadersEnabled=false;PixelShadersEnabled=false;
    FixedFunctionVertexMode=false;SoftwareVertexShaderEnable=false;SoftwareFixedFunctionEnable=false;
    ConstantLayouts().resize(1,0);
    ConstantLayouts()[0]=&NullConstantLayout;
    CurrentVSConstantLayout=&NullConstantLayout;
    CurrentVSConstantLayoutIndex=-1;RenderManager=render;D3DDevice=device;
    for(unsigned long i=0;i<Resources().size();++i)Resources()[i]->Initialise(*this);
    WorldIdentity(WorldMatrix);WorldMatrixIsIdentity=true;
    Identity(ViewMatrix);Identity(ProjectionMatrix);
    MultisampleViewPortScale=1.0f;TextureTransformCount=0;Identity(TextureTransforms[0]);
    LightCount=0;AllocatedLights=0;AllocatedLightSize=0;
    AmbientColour.R=1;AmbientColour.G=1;AmbientColour.B=1;AmbientColour.A=1;
    FogColour.R=0;FogColour.G=0;FogColour.B=0;FogColour.A=0;
    TextureFactor=CRGBColour(255,255,255,255);
    { CCharString name("PSCONST_INPUT_FACTOR_0",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_INPUT_FACTOR_1",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_OUTPUT_FACTOR",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_MAX_FOG_ALPHA",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_USER_0",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_USER_1",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_USER_2",-1); PixelShaderConstantNames().push_back(name); }
    { CCharString name("PSCONST_USER_3",-1); PixelShaderConstantNames().push_back(name); }
    C4DVector zero;zero.X=0;zero.Y=0;zero.Z=0;zero.W=0;
    PixelShaderConstantTable().resize(9,zero);
    PixelShaderConstantTable()[0]=C4DVector(1,1,1,1);
    PixelShaderConstantTable()[1]=C4DVector(1,1,1,1);
    PixelShaderConstantTable()[2]=C4DVector(1,1,1,1);
    updateFlags3D8=0;
}
