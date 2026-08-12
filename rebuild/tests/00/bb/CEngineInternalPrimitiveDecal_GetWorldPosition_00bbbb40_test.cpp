#include <stdio.h>

struct C3DVector {
    float x;
    float y;
    float z;
};

struct CEngineInternalPrimitiveDecal {
    char pad[0x54];
    C3DVector m_worldPosition;

    bool GetWorldPosition(C3DVector& out);
};

bool CEngineInternalPrimitiveDecal::GetWorldPosition(C3DVector& out)
{
    out.x = m_worldPosition.x;
    out.y = m_worldPosition.y;
    out.z = m_worldPosition.z;
    return true;
}

int main()
{
    CEngineInternalPrimitiveDecal decal;
    decal.m_worldPosition.x = 1.5f;
    decal.m_worldPosition.y = -2.25f;
    decal.m_worldPosition.z = 100.0f;

    C3DVector out;
    out.x = out.y = out.z = 0.0f;
    bool r = decal.GetWorldPosition(out);

    if (r && out.x == 1.5f && out.y == -2.25f && out.z == 100.0f) {
        printf("DECAL_WORLDPOS_OK\n");
        return 0;
    }
    printf("DECAL_WORLDPOS_FAIL\n");
    return 1;
}