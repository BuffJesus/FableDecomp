struct AutoTinyVirtualObject
{
    virtual ~AutoTinyVirtualObject();
};
void __fastcall CEngine_Release(AutoTinyVirtualObject* object)
{
    delete object;
}