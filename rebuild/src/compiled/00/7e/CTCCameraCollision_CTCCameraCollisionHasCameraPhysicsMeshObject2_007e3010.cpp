struct CTCCameraCollision {
    void* field_0;
    void* field_4;
    void* field_8;
    struct Inner { char pad[0x28]; void* field_28; } *field_c;

    unsigned char __fastcall HasCameraPhysicsMeshObject_2(void* /*edx*/);
};

unsigned char __fastcall CTCCameraCollision::HasCameraPhysicsMeshObject_2(void* /*edx*/)
{
    return field_c->field_28 != 0;
}