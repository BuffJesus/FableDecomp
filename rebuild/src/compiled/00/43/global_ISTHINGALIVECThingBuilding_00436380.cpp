struct CThing { char pad[0x91]; unsigned char flags; };
struct ThingRef { CThing* p; };

bool __fastcall IS_THING_ALIVE(const ThingRef* self)
{
    CThing* t = self->p;
    if (t != 0 && !(t->flags & 1))
        return true;
    return false;
}