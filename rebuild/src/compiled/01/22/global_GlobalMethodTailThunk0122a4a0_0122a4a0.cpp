struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_0122a4a0()
{
    g_AutoTinyThunkObject.Invoke();
}