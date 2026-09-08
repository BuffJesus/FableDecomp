struct C3DVector { float X; float Y; float Z; };
struct CPlane {
    C3DVector Normal;
    float Distance;
    bool Initialise(const C3DVector* normal, float distance);
};
bool CPlane::Initialise(const C3DVector* normal, float distance)
{
    Normal = *normal;
    Distance = distance;
    return true;
}