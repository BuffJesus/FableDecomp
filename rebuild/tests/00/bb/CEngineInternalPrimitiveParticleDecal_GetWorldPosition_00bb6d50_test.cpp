#include <cstdio>

struct C3DVector {
    float x, y, z;
};

struct CEngineInternalPrimitiveParticleDecal {
    char pad[0x48];
    C3DVector m_pos;

    bool GetWorldPosition(C3DVector& out);
};

bool CEngineInternalPrimitiveParticleDecal::GetWorldPosition(C3DVector& out)
{
    out.x = m_pos.x;
    out.y = m_pos.y;
    out.z = m_pos.z;
    return true;
}

int main()
{
    CEngineInternalPrimitiveParticleDecal d;
    d.m_pos.x = 1.5f;
    d.m_pos.y = -2.25f;
    d.m_pos.z = 42.0f;

    C3DVector out;
    out.x = out.y = out.z = 0.0f;

    bool r = d.GetWorldPosition(out);

    if (r && out.x == 1.5f && out.y == -2.25f && out.z == 42.0f) {
        printf("GETWORLDPOS_OK\n");
        return 0;
    }
    printf("GETWORLDPOS_FAIL\n");
    return 1;
}