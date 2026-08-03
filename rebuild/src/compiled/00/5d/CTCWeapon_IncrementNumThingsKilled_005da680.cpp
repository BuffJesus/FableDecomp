struct Thing {
    unsigned char pad_00[0x6c];
    unsigned char flags6c;
    unsigned char pad_6d[0x91 - 0x6d];
    unsigned char flags91;
};

struct CTCWeapon {
    unsigned char pad_00[0x1c];
    int numThingsKilled;
    __declspec(dllexport) void IncrementNumThingsKilled(Thing* thing);
};

void CTCWeapon::IncrementNumThingsKilled(Thing* thing)
{
    if (thing == 0) return;
    if (thing->flags91 & 0x1) return;
    if (!(thing->flags6c & 0x8)) return;
    ++this->numThingsKilled;
}