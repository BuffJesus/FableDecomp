#pragma optimize("s",on)

struct CTCBitsPair
{
    unsigned long words[9];
    unsigned long Test(unsigned long bit);
};

struct CTCMapPair
{
    int* begin;
    int* end;
    int* LowerBound(const int* key);
};

struct CTCInterface4
{
    unsigned char unused00[0x20];
    CTCBitsPair bits;
    CTCMapPair map;
    bool CTC_FindInterface_4(int* output);
};

bool CTCInterface4::CTC_FindInterface_4(int* output)
{
    unsigned long tested = bits.Test(4);
    bool result;
    if ((unsigned char)tested != 0) {
        int key = 4;
        CTCMapPair* current_map = &(map);
        int* entry = current_map->LowerBound(&key);
        int* end = current_map->end;
        if (entry == end || *entry > 4) entry = end;
        *output = entry[1];
        result = true;
    } else {
        result = false;
    }
    return result;
}

struct CTCInterfaceE8
{
    unsigned char unused00[0x20];
    CTCBitsPair bits;
    CTCMapPair map;
    bool CTCBase_FindInterfaceType_0xe8(int* output);
};

bool CTCInterfaceE8::CTCBase_FindInterfaceType_0xe8(int* output)
{
    register int key = 0xe8;
    unsigned long tested = bits.Test(key);
    bool result;
    if ((unsigned char)tested != 0) {
        CTCMapPair* current_map = &(map);
        int local_key = key;
        int* entry = current_map->LowerBound(&local_key);
        int* end = current_map->end;
        if (entry == end || *entry > key) entry = end;
        *output = entry[1];
        result = true;
    } else {
        result = false;
    }
    return result;
}