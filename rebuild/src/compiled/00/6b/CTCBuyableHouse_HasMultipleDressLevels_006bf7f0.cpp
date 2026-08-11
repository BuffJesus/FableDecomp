// CTCBuyableHouse::HasMultipleDressLevels  @ 0x006bf7f0

struct DressData {
    char pad0[0x28];
    int* first;   // +0x28
    int* last;    // +0x2c
};

struct CTCBuyableHouse {
    char pad0[0xc];
    DressData* dress;  // +0x0c
};

bool __fastcall HasMultipleDressLevels(CTCBuyableHouse* self)
{
    DressData* d = self->dress;
    if (d != 0)
    {
        unsigned int count = (unsigned int)(d->last - d->first);
        if (count > 1)
            return true;
    }
    return false;
}