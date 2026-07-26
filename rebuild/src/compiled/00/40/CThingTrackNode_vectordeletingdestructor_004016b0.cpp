struct CThingTrackNode {
    void* vptr;
    char pad[0x18 - 4];
    int member18;
};

extern const void* CThingTrackNode_vftable;
extern void __fastcall Member599270(void* self);
extern void __fastcall Member18Dtor(void* p);
extern void __fastcall BaseDtor(void* self);
extern void __cdecl EngineFree(void* p);

void* __fastcall CThingTrackNode_vector_deleting_destructor(CThingTrackNode* self, int edx, unsigned int flags)
{
    (void)edx;
    self->vptr = (void*)&CThingTrackNode_vftable;
    Member599270(self);
    Member18Dtor((char*)self + 0x18);
    BaseDtor(self);
    if (flags & 1) {
        EngineFree(self);
    }
    return self;
}