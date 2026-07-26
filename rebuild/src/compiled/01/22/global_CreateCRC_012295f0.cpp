struct AutoTinyReverseLoopTarget
{
    unsigned char storage[4];
    void Invoke();
};
extern AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects[18];
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects + 18;
    for (int remaining = 18; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}