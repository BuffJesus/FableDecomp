#include "fable_bounding_volumes.h"
#include "engine/CMatrix3x4.h"
static __forceinline C3DVector sum(const C3DVector& a,const C3DVector& b)
{
    return C3DVector(a.X+b.X,a.Y+b.Y,a.Z+b.Z);
}
void C3DBoundingBox::CalculateMatrix(CMatrix3x4& matrix) const
{
    C3DVector center=sum(MaxCoord,MinCoord)*0.5f;
    C3DVector half=(MaxCoord-MinCoord)*0.5f;
    if(half.X<DELTA_NOTIONAL_ZERO)half.X=DELTA_NOTIONAL_ZERO;
    if(half.Y<DELTA_NOTIONAL_ZERO)half.Y=DELTA_NOTIONAL_ZERO;
    if(half.Z<DELTA_NOTIONAL_ZERO)half.Z=DELTA_NOTIONAL_ZERO;
    matrix.E11=half.X;
    matrix.E33=half.Z;
    matrix.E12=0.0f;
    matrix.E13=0.0f;
    matrix.E41=center.X;
    matrix.E21=0.0f;
    matrix.E23=0.0f;
    matrix.E31=0.0f;
    matrix.E42=center.Y;
    matrix.E32=0.0f;
    matrix.E22=half.Y;
    matrix.E43=center.Z;
}
