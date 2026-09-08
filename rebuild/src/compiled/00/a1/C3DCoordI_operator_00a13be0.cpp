struct C3DVector { float X; float Y; float Z; };
struct C3DCoordI { long X; long Y; long Z; C3DCoordI& operator+=(const C3DVector& offset); };

C3DCoordI& C3DCoordI::operator+=(const C3DVector& offset)
{
    X += offset.X;
    Y += offset.Y;
    Z += offset.Z;
    return *this;
}