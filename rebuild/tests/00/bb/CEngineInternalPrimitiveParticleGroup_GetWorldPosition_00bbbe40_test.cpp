#include <cstdio>

struct C3DVector {
    float x, y, z;
};

struct CEngineInternalPrimitiveParticleGroup {
    char pad[0x5c];
    C3DVector m_position;

    bool GetWorldPosition(C3DVector &out);
};

bool CEngineInternalPrimitiveParticleGroup::GetWorldPosition(C3DVector &out)
{
    out.x = m_position.x;
    out.y = m_position.y;
    out.z = m_position.z;
    return true;
}

int main()
{
    CEngineInternalPrimitiveParticleGroup g;
    g.m_position.x = 1.5f;
    g.m_position.y = -2.25f;
    g.m_position.z = 100.0f;

    C3DVector out;
    out.x = out.y = out.z = 0.0f;
    bool r = g.GetWorldPosition(out);

    if (r && out.x == 1.5f && out.y == -2.25f && out.z == 100.0f) {
        printf("GETWORLDPOS_OK\n");
        return 0;
    }
    printf("GETWORLDPOS_FAIL\n");
    return 1;
}