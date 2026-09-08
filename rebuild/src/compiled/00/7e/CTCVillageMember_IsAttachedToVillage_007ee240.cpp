struct CThing;
struct CIntelligentPointerThing {
    CThing* Object;
    void* ReferenceInfo;
    CThing* Get() const;
};
struct CTCVillageMember {
    unsigned char BaseSubobject[0x10];
    CIntelligentPointerThing PVillage;
    unsigned __int64 VillageUID;
    bool IsAttachedToVillage() const;
};
bool CTCVillageMember::IsAttachedToVillage() const
{
    if (PVillage.Get())
        return true;
    return VillageUID != 0;
}