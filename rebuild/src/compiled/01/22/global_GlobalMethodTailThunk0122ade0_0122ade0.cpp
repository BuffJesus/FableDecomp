struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_0122ade0()
{
    g_AutoTinyThunkObject.Invoke();
}