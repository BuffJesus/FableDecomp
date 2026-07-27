#pragma optimize("t", on)
extern void __fastcall AutoTinyFirstMemberDestructor(void* member);
extern void __fastcall AutoTinySecondMemberDestructor(void* member);
extern void __fastcall AutoTinyCompositeDestructor(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 108);
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 72);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}