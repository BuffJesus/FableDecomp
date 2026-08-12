struct ISized {
    virtual ~ISized();
    virtual long Sizeof();
};

struct CPhysicsFacesSegmentBranch {
    char pad[0x14];
    ISized* m14;   // +0x14
    ISized* m18;   // +0x18
    long Sizeof();
};

long CPhysicsFacesSegmentBranch::Sizeof() {
    return this->m14->Sizeof() + this->m18->Sizeof() + 0x1c;
}