// QuadricOptimiserInternals::CQuadric (Ego_r names): EvaluateError (retail 0x00AC6D50), operator+= (0x00AC6E80),
// operator*= (0x00AC6EF0) and Initialise (0x00AC7050). A quadric holds the coefficients of
// Q(x, y, z) = AA x^2 + 2 AB xy + 2 AC xz + 2 AD x + BB y^2 + 2 BC yz + 2 BD y + CC z^2 + 2 CD z + DD and a weight.
#include "fable_quadric_optimiser.h"

namespace QuadricOptimiserInternals
{
double CQuadric::EvaluateError(const C3DVector& v) const
{
    float x = v.X, y = v.Y, z = v.Z;
    return z * z * CC + y * z * 2 * BC + y * y * BB + x * z * 2 * AC + x * y * 2 * AB + z * 2 * CD + y * 2 * BD +
           x * 2 * AD + x * x * AA + DD;
}

void CQuadric::operator+=(const CQuadric& other)
{
    AA += other.AA; AB += other.AB; AC += other.AC; AD += other.AD;
    BB += other.BB; BC += other.BC; BD += other.BD;
    CC += other.CC; CD += other.CD;
    DD += other.DD;
    Weight += other.Weight;
}

void CQuadric::operator*=(double scale)
{
    AA *= scale; AB *= scale; AC *= scale; AD *= scale;
    BB *= scale; BC *= scale; BD *= scale;
    CC *= scale; CD *= scale;
    DD *= scale;
    Weight *= scale;
}

void CQuadric::Initialise(const C3DVector& normal, const C3DVector& point, float weight)
{
    float a = normal.X, b = normal.Y, c = normal.Z;
    float d = -(point.Z * normal.Z + point.Y * normal.Y + point.X * normal.X);
    AA = a * a; AB = a * b; AC = a * c; AD = a * d;
    BB = b * b; BC = b * c; BD = b * d;
    CC = c * c; CD = c * d;
    DD = d * d;
    Weight = weight;
}

}
