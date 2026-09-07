#pragma optimize("s",on)

struct InterfaceBits
{
    unsigned long words[9];
    unsigned long Test(unsigned long bit);
};

struct InterfaceMap
{
    int* begin;
    int* end;
    int* LowerBound(const int* key);
};

struct InterfaceContainer
{
    unsigned char unused00[0x20];
    InterfaceBits bits;
    InterfaceMap map;

    bool CTC_FindInterface_0x11(int* output);
};

bool InterfaceContainer::CTC_FindInterface_0x11(int* output)
{
    unsigned long tested = bits.Test(0x11);
    bool result;
    if ((unsigned char)tested != 0) {
        int key = 0x11;
        InterfaceMap* current_map = &(map);
        int* entry = current_map->LowerBound(&key);
        int* end = current_map->end;
        if (entry == end || *entry > 0x11) {
            entry = end;
        }
        *output = entry[1];
        result = true;
    } else {
        result = false;
    }
    return result;
}