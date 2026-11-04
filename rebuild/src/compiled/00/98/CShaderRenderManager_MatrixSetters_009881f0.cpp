#include "fable_shader_render_manager.h"

// The retail assignment preserves raw float bits and its ordered loads/stores.
// CMatrix3x4's twelve floats are in E11,E12,E13,...,E41,E42,E43 order.
static __forceinline void copyTransposed(CMatrix4x4& destination,const CMatrix3x4& source)
{
    unsigned long* out=reinterpret_cast<unsigned long*>(&destination);
    const unsigned long* in=reinterpret_cast<const unsigned long*>(&source);
    out[0]=in[0];out[1]=in[3];out[2]=in[6];out[3]=in[9];
    out[4]=in[1];out[5]=in[4];out[6]=in[7];out[7]=in[10];
    out[8]=in[2];out[9]=in[5];out[10]=in[8];out[11]=in[11];
    out[12]=0;out[13]=0;out[14]=0;out[15]=0x3f800000;
}
void CShaderRenderManager::SetWorldTransform(const CMatrix3x4& transform)
{
    copyTransposed(WorldMatrix,transform);
    updateFlags3D8|=0xc990;
    WorldMatrixIsIdentity=false;
}
void CShaderRenderManager::SetViewTransform(const CMatrix3x4& transform)
{
    copyTransposed(ViewMatrix,transform);
    updateFlags3D8|=0xc990;
}
void CShaderRenderManager::SetProjectionTransform(const CMatrix4x4& transform)
{
    ProjectionMatrix=transform;
    ProjectionMatrix.Transpose();
    updateFlags3D8|=0xc990;
}
