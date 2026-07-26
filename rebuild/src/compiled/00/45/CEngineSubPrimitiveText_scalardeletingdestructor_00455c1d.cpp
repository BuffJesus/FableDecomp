#pragma optimize("s",on)
struct CSubObj { ~CSubObj(); };
struct CEngineSubPrimitiveText {
    char pad[0x3c];
    CSubObj sub;
    ~CEngineSubPrimitiveText();
    void * scalar_deleting_destructor(unsigned int flags);
};

void * CEngineSubPrimitiveText::scalar_deleting_destructor(unsigned int flags)
{
    this->sub.~CSubObj();
    this->~CEngineSubPrimitiveText();
    if (flags & 1)
        ::operator delete(this);
    return this;
}