#include <cstdio>

struct C3DVector {
    float x, y, z;
};

struct CCombatAction_GenericStrikeResponseBase {
    char pad[0x128];
    C3DVector m_hitDir;
    void SetHitDirection(C3DVector v);
};

void CCombatAction_GenericStrikeResponseBase::SetHitDirection(C3DVector v)
{
    m_hitDir = v;
}

int main()
{
    CCombatAction_GenericStrikeResponseBase obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;
    C3DVector v;
    v.x = 1.5f; v.y = -2.25f; v.z = 42.0f;
    obj.SetHitDirection(v);
    bool ok = obj.m_hitDir.x == 1.5f && obj.m_hitDir.y == -2.25f && obj.m_hitDir.z == 42.0f;
    if (ok) {
        printf("HITDIR_OK\n");
        return 0;
    }
    printf("HITDIR_FAIL\n");
    return 1;
}