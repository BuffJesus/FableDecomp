// CGameScriptThing::GetHomePos @ 008cfe30  -- by-value C3DVector return (sret).
//
// The ordering that matters: retail loads the SOURCE MEMBERS first and the sret pointer
// second (`mov edx,[ecx+0x34]; mov eax,[esp+4]; mov [eax],edx; ...`). Neither `return
// m_Pos;` nor a by-value constructor reproduces that -- both load the sret pointer first.
// A constructor taking const REFERENCES does: each member is read through its reference
// before the store, which is what pins the retail instruction order.

struct C3DVector
{
    float x, y, z;
    C3DVector(const float& xx, const float& yy, const float& zz) : x(xx), y(yy), z(zz) {}
};

class CGameScriptThing
{
public:
    virtual C3DVector GetHomePos() const;
    char m_Pad[0x30];
    float m_x, m_y, m_z;
};

C3DVector CGameScriptThing::GetHomePos() const
{
    return C3DVector(m_x, m_y, m_z);
}
