struct IData {
    virtual void Release(int flag) = 0;
};

void __fastcall DeleteData(IData *self)
{
    if (self)
        self->Release(1);
}