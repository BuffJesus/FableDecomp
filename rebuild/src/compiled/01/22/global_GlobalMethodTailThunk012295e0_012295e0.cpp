struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_012295e0()
{
    g_AutoTinyThunkObject.Invoke();
}