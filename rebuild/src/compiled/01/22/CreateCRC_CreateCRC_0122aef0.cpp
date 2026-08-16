struct AutoTinyReverseLoopTarget
{
    unsigned char storage[4];
    void Invoke();
};
extern AutoTinyReverseLoopTarget g_AutoTinyReverseLoopObjects[3];
void __fastcall CreateCRC()
{
    AutoTinyReverseLoopTarget* cursor =
        g_AutoTinyReverseLoopObjects + 3;
    for (int remaining = 3; remaining != 0; --remaining)
    {
        --cursor;
        cursor->Invoke();
    }
}