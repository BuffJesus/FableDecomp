struct AutoTinyVirtualObject
{
    virtual ~AutoTinyVirtualObject();
};
void __fastcall DeleteData(AutoTinyVirtualObject* object)
{
    delete object;
}