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
    out = m_pos;
    return true;
}