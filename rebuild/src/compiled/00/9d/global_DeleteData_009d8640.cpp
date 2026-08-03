struct IDataHolder
{
    virtual void Method0();
    virtual void Method1();
    virtual void Release(int flag);
};

void __fastcall DeleteData(IDataHolder* self)
{
    if (self != 0)
        self->Release(1);
}