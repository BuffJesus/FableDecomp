#include "fable_shader_render_manager.h"
#include "fable_vertex_shader.h"
#include "fable_display_viewport.h"

void CShaderRenderManager::ApplyVertexShader(const CVertexShader& shader,bool)
{
    const bool expectedMode=FixedFunctionVertexMode?SoftwareFixedFunctionEnable:SoftwareVertexShaderEnable;
    const bool actualMode=RenderManager->DeviceState.SoftwareVertexProcessing.CurrentState!=0;
    if(expectedMode!=actualMode)
        CurrentVertexShader=0;
    if(CurrentVertexShader==shader.Data->VertexShader &&
       CurrentVertexShaderVertexDeclairation==shader.Data->VertexDeclaration)
        return;
    CurrentVertexShader=shader.Data->VertexShader;
    CurrentVertexShaderVertexDeclairation=shader.Data->VertexDeclaration;
    RenderManager->FixedFunctionPipelineEnabled=false;
    typedef HRESULT (__stdcall *SetShaderCall)(IDirect3DDevice9*,void*);
    void** methods=*reinterpret_cast<void***>(D3DDevice);
    reinterpret_cast<SetShaderCall>(methods[92])(D3DDevice,CurrentVertexShader);
    methods=*reinterpret_cast<void***>(D3DDevice);
    reinterpret_cast<SetShaderCall>(methods[87])(D3DDevice,CurrentVertexShaderVertexDeclairation);
}
