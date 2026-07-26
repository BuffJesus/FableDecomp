struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_012299d0()
{
    g_AutoTinyThunkObject.Invoke();
}