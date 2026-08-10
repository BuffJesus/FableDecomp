struct CIDrawEnvironment;
struct CWorld {
    void* pad0;
    void* pad4;
    CIDrawEnvironment* env8;
};
CIDrawEnvironment* __fastcall DrawGetEnvironment(CWorld* self) {
    return self->env8;
}