#pragma optimize("s",on)
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);
};
struct CVectorStringPair {
    C3DVector first;
    CCharString second;
    CVectorStringPair& operator=(const CVectorStringPair& other);
};
CVectorStringPair& CVectorStringPair::operator=(const CVectorStringPair& other)
{
    first.X = other.first.X;
    first.Y = other.first.Y;
    first.Z = other.first.Z;
    second = other.second;
    return *this;
}