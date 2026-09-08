struct CThing;
struct CIntelligentPointerThing {
    CThing* Object;
    void* ReferenceInfo;
    CThing* Get() const;
};

struct CTCCrateStack {
    unsigned char BaseSubobject[0x0c];
    CIntelligentPointerThing FirstCrate;
    CIntelligentPointerThing SecondCrate;

    long GetNumCrates() const;
};

long CTCCrateStack::GetNumCrates() const
{
    if (SecondCrate.Get())
        return 2;
    return FirstCrate.Get() != 0;
}