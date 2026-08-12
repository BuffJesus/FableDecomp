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
    out = m_position;
    return true;
}