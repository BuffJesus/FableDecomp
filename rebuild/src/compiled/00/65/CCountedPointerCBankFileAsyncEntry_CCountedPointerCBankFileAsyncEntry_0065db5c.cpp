// Intelligent-pointer copy: take the target and the refcount block from the
// source, then bump the count when the block exists. VC7.1 schedules the
// second store ahead of the branch. __fastcall this=ecx (ret 4).
struct RefCount { int count; };
struct Ptr {
    void* target;
    RefCount* refs;
};
struct T {
    void* target;
    RefCount* refs;
    T* Copy(const Ptr& other);
};
T* T::Copy(const Ptr& other) {
    this->target = other.target;
    this->refs = other.refs;
    if (this->refs) ++this->refs->count;
    return this;
}