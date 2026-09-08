#pragma optimize("s",on)
struct C3DVector { float X; float Y; float Z; };
struct CCharString {
    void* Data;
    CCharString& operator=(const CCharString& source);
};
struct C3DVectorCharStringPair {
    C3DVector first;
    CCharString second;
    C3DVectorCharStringPair& operator=(const C3DVectorCharStringPair& source);
};
C3DVectorCharStringPair& C3DVectorCharStringPair::operator=(const C3DVectorCharStringPair& source)
{
    first = source.first;
    second = source.second;
    return *this;
}