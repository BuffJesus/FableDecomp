#include <cstdio>

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

int main()
{
    CGameScriptThing t;
    t.m_Pos.x = 1.5f;
    t.m_Pos.y = 2.5f;
    t.m_Pos.z = 3.5f;
    C3DVector r = t.GetHomePos();
    if (r.x == 1.5f && r.y == 2.5f && r.z == 3.5f)
        printf("GETHOMEPOS_OK\n");
    else
        printf("GETHOMEPOS_FAIL\n");
    return 0;
}