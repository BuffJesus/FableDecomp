struct CThingInner { char pad[0x91]; unsigned char flags; };
struct CThingHolder { CThingInner* inner; };

bool __fastcall IS_THING_ALIVE(const CThingHolder* self)
{
    const CThingInner* p = self->inner;
    if (p != 0 && (p->flags & 1) == 0)
        return true;
    return false;
}