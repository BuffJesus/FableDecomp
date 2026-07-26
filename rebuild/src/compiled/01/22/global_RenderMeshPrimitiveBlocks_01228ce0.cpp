struct AutoTinyMethodOptionalTarget
{
    void Invoke();
};
extern AutoTinyMethodOptionalTarget g_AutoTinyMethodOptionalObject;
extern void* g_AutoTinyMethodOptionalPointer;
extern void __cdecl AutoTinyMethodOptionalCall(void* object);
void __fastcall RenderMeshPrimitiveBlocks()
{
    g_AutoTinyMethodOptionalObject.Invoke();
    if (g_AutoTinyMethodOptionalPointer != 0)
        AutoTinyMethodOptionalCall(g_AutoTinyMethodOptionalPointer);
}