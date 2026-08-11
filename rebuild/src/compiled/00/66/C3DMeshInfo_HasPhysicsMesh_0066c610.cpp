struct C3DMeshInfo {
    char pad[0x28];
    void* physicsMesh;
    bool HasPhysicsMesh();
};

bool C3DMeshInfo::HasPhysicsMesh()
{
    return this->physicsMesh != 0;
}