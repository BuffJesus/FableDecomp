#include "fable_display_viewport.h"
#include "fable_matrix4x4.h"
static __forceinline void expandMatrix(CMatrix4x4& destination,const CMatrix3x4& source)
{
    unsigned long* out=reinterpret_cast<unsigned long*>(&destination);
    const unsigned long* in=reinterpret_cast<const unsigned long*>(&source);
    out[0]=in[0];out[1]=in[1];out[2]=in[2];
    out[4]=in[3];out[5]=in[4];out[6]=in[5];
    out[8]=in[6];out[9]=in[7];out[10]=in[8];
    out[12]=in[9];out[13]=in[10];out[14]=in[11];
    out[3]=0;out[7]=0;out[11]=0;out[15]=0x3f800000;
}
void CRenderManagerCore::SetViewTransform(const CMatrix3x4& transform)
{
    CMatrix4x4 matrix;
    expandMatrix(matrix,transform);
    PD3DDevice->SetTransform(2,reinterpret_cast<const D3DMATRIX*>(&matrix)); // D3DTS_VIEW
}
void CRenderManagerCore::SetWorldTransform(const CMatrix3x4& transform)
{
    CMatrix4x4 matrix;
    expandMatrix(matrix,transform);
    PD3DDevice->SetTransform(256,reinterpret_cast<const D3DMATRIX*>(&matrix)); // D3DTS_WORLD
}
void CRenderManagerCore::SetPerspectiveTransform(float nearZ,float farZ,float zStart,float zEnd)
{
    CMatrix4x4 matrix;
    matrix.E11=1.0f;matrix.E12=0.0f;matrix.E13=0.0f;matrix.E14=0.0f;
    matrix.E21=0.0f;matrix.E22=1.0f;matrix.E23=0.0f;matrix.E24=0.0f;
    matrix.E31=0.0f;matrix.E32=0.0f;
    matrix.E43=(zStart-zEnd)*nearZ*farZ/(farZ-nearZ);
    matrix.E33=zStart-matrix.E43/nearZ;
    matrix.E34=1.0f;
    matrix.E41=0.0f;matrix.E42=0.0f;matrix.E44=0.0f;
    PD3DDevice->SetTransform(3,reinterpret_cast<const D3DMATRIX*>(&matrix)); // D3DTS_PROJECTION
}
