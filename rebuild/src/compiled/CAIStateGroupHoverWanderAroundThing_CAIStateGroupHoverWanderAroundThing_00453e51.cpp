struct Member;
extern "C" void __fastcall Member_dtor(Member* m);
struct Base;
extern "C" void __fastcall Base_dtor(Base* b);
extern void* const g_vtbl[1];
struct CAIStateGroup_HoverWanderAroundThing {
    void* vt;
    char pad[0x24];
    Member* m28;
};
void __fastcall CAIStateGroup_HoverWanderAroundThing_dtor(CAIStateGroup_HoverWanderAroundThing* self)
{
    Member_dtor((Member*)((char*)self + 0x28));
    self->vt = (void*)g_vtbl;
    Base_dtor((Base*)self);
}