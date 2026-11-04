#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_matrix4x4_simd.h"
#include "fable_d3d9.h"

typedef HRESULT (__stdcall *SetConstantsCall)(IDirect3DDevice9*,long,const void*,unsigned long);

// Retail 0x00988A50 (Ego_r ?UpdateCombinedProjectionTransform@CShaderRenderManager@@AAEXXZ): world*view,
// then *projection, uploaded as four constant registers at the layout's combined-projection range.
void CShaderRenderManager::UpdateCombinedProjectionTransform()
{
    __declspec(align(16)) CMatrix4x4 worldView;
    worldView.AlignedSetMultiplyTransposed(WorldMatrix,ViewMatrix);
    CombinedProjectionMatrix.AlignedSetMultiplyTransposed(worldView,ProjectionMatrix);
    void** methods=*reinterpret_cast<void***>(D3DDevice);
    reinterpret_cast<SetConstantsCall>(methods[94])(D3DDevice,CurrentVSConstantLayout->CombinedProjectionMatrix.Offset,&CombinedProjectionMatrix,4);
    updateFlags3D8 &= ~0x80u;
}
