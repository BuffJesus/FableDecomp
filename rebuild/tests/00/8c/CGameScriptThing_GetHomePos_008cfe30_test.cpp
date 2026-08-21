#include <cstdio>

// Declares only -- the behaviour exe links this test object together with
// CGameScriptThing_GetHomePos_008cfe30.cpp, so the reconstruction itself is what runs.
// (The previous version of this test DEFINED its own GetHomePos and therefore proved
// nothing about the landed source.)

struct C3DVector
{
    float x, y, z;
    C3DVector(const float& xx, const float& yy, const float& zz);
};

class CGameScriptThing
{
public:
    virtual C3DVector GetHomePos() const;
    char m_Pad[0x30];
    float m_x, m_y, m_z;
};

int main()
{
    CGameScriptThing t;
    t.m_x = 1.5f;
    t.m_y = 2.5f;
    t.m_z = 3.5f;
    C3DVector r = t.GetHomePos();
    if (r.x == 1.5f && r.y == 2.5f && r.z == 3.5f)
        printf("GETHOMEPOS_OK\n");
    else
        printf("GETHOMEPOS_FAIL\n");
    return 0;
}
