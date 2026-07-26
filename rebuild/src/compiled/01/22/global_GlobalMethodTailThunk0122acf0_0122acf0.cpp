struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject;
void __fastcall GlobalMethodTailThunk_0122acf0()
{
    g_AutoTinyThunkObject.Invoke();
}