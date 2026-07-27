struct AutoTinyVirtualObject
{
    virtual ~AutoTinyVirtualObject();
};
void __fastcall CIEngine_Release(AutoTinyVirtualObject* object)
{
    delete object;
}