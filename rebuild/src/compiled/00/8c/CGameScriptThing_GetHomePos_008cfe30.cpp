struct C3DVector
{
    float x;
    float y;
    float z;
    C3DVector() {}
    C3DVector(const C3DVector& o) { x = o.x; y = o.y; z = o.z; }
};

class CGameScriptThing
{
public:
    virtual C3DVector GetHomePos() const;
    char m_Pad[0x30];
    C3DVector m_Pos;
};

C3DVector CGameScriptThing::GetHomePos() const
{
    return m_Pos;
}