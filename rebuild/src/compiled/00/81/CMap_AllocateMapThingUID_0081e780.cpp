// Ego_r identifies this counter as ThingUIDCount at +0x1d40. Retail's
// preceding CMap state is sixteen bytes smaller and places it at +0x1d30.
struct CMapRetail {
    unsigned char EarlierState[0x1d30];
    unsigned __int64 ThingUIDCount;
    unsigned __int64 AllocateMapThingUID();
};
unsigned __int64 CMapRetail::AllocateMapThingUID()
{
    return ThingUIDCount++;
}