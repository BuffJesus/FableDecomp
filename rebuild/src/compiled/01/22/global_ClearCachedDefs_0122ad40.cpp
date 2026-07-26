struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First();
    void Second();
};
extern AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject;
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject.value = 0x012a20fc;
    g_AutoTinyFieldMethodsObject.First();
    g_AutoTinyFieldMethodsObject.Second();
}