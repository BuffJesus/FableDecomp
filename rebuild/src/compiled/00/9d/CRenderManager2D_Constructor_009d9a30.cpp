#include "fable_retail_banks.h"
#include "fable_display_viewport.h"
#include "fable_shader_resource.h"
#include "fable_shader_render_manager.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
#include "fable_render2d_shader.h"
CRenderManager2D::CRenderManager2D(CDisplayManager& display,
    const CWideString& shaderPath,unsigned long flags)
    : CRenderManagerCore(display,flags)
{
    if (g_FableBankFileManager_013CA79C.UseRetailBanks) {
        CWideString platformPath(L"pc\\");
        g_FableBankFileManager_013CA79C.OpenRetailBank(
            shaderPath + platformPath + L"shaders.big",false);
    }
    CCharString bankName("SHADERS_BBBLIB",-1);
    if (g_FableBankFileManager_013CA79C.BankExists(bankName)) {
        ShaderDataBank.OpenReadOnly(bankName,0x48);
        ShaderResource = FableShaderRenderManager_013bc470.CreateResource();
        ShaderResource->OpenShaderBank(&ShaderDataBank);
        ShaderResource->SetVertexShaderDeclaration(Fable2DShaderDeclaration_0129D708);
        ShaderResource->SetVertexFormat(static_cast<EVertexType>(0x1c4));
        CCharString shaderName("VSHADER_BBBLIB_2D",-1);
        VertexShader2D = ShaderResource->GetVertexShader(shaderName);
    }
    AlternateRenderTargetClearEnabled = (flags & 1) != 0;
}
