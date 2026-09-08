struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};
struct CAnimComponentBase {
    virtual ~CAnimComponentBase();
    long ReferenceCount;
    CAnimComponentBase() : ReferenceCount(1) {}
};
struct CAnimComponentFollowingOnAnim : CAnimComponentBase {
    CDefString NextAnimName;
    static CAnimComponentBase* Alloc();
};
CAnimComponentBase* CAnimComponentFollowingOnAnim::Alloc()
{
    return new CAnimComponentFollowingOnAnim;
}