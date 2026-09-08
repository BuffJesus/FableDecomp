struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};
struct CAnimComponentBase {
    virtual ~CAnimComponentBase();
    long ReferenceCount;
    CAnimComponentBase() : ReferenceCount(1) {}
};
struct CAnimComponentFollowingOnAnimFilter : CAnimComponentBase {
    CDefString NextFilter;
    static CAnimComponentBase* Alloc();
};
CAnimComponentBase* CAnimComponentFollowingOnAnimFilter::Alloc()
{
    return new CAnimComponentFollowingOnAnimFilter;
}