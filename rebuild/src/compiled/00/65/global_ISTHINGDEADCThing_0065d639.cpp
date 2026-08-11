struct CThing {
    char pad[0x91];
    unsigned char flags;
};

bool __fastcall IS_THING_DEAD(CThing** ppThing)
{
    CThing* t = *ppThing;
    if (t != 0 && (t->flags & 1) == 0)
        return false;
    return true;
}