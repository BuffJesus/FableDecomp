class CEngineInternalPrimitiveParticleGroup { public: bool PrimTypeMatchesGroup(long primitiveType); };

bool CEngineInternalPrimitiveParticleGroup::PrimTypeMatchesGroup(long primitiveType)
{
    return primitiveType == 24 ||
           primitiveType == 28 ||
           primitiveType == 26 ||
           primitiveType == 27 ||
           primitiveType == 29 ||
           primitiveType == 7 ||
           primitiveType == 20 ||
           primitiveType == 30;
}