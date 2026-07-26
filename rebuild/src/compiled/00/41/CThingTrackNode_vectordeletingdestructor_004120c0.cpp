struct CThingTrackNode {
    void* vptr;      // +0x0
    char pad4[0x0C]; // +0x4 .. +0x10
    void* member10;  // +0x10
    char pad14[0x0C];// +0x14 .. +0x20
    char member20;   // +0x20 (subobject destructed via ecx=esi+0x20)
};

extern const void* CThingTrackNode_vtbl[];
extern void __fastcall Cleanup_5d72d0(void* self);
extern void __fastcall SubDtor_589450(void* sub);
extern void __cdecl Free_7ec954(void* p);
extern void __fastcall BaseDtor_588240(void* self);
extern void __cdecl OpDelete_7ec8fc(void* p);

void* __fastcall CThingTrackNode_vector_deleting_destructor(CThingTrackNode* self, void* /*edx*/, unsigned int flags)
{
    self->vptr = (void*)CThingTrackNode_vtbl;
    Cleanup_5d72d0(self);
    SubDtor_589450(&self->member20);
    if (self->member10)
        Free_7ec954(self->member10);
    BaseDtor_588240(self);
    if (flags & 1)
        OpDelete_7ec8fc(self);
    return self;
}