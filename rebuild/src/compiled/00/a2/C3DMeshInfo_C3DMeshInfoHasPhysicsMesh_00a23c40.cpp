struct C3DMeshInfo
{
    int unused00[15];
    int physicsMeshIndex; // +0x3c

    bool __fastcall HasPhysicsMesh(void* edx_unused);
};

bool __fastcall C3DMeshInfo::HasPhysicsMesh(void* edx_unused)
{
    return physicsMeshIndex != -1;
}