struct CNavRegion { char pad[384]; };

struct CNavSwitchableLeafNode {
    char header[0x18];              // 0x00
    CNavRegion regions[1];          // 0x18, stride 384
    char gap[0x318 - 0x18 - 384];   // pad to 0x318
    int  index;                     // 0x318
    char gap2[0x33c - 0x318 - 4];   // pad to 0x33c
    char flag;                      // 0x33c
};

CNavRegion * __fastcall GetRegion(CNavSwitchableLeafNode *self)
{
    if (self->flag)
        return 0;
    return &self->regions[self->index];
}