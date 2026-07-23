#pragma optimize("s",on)
struct CBase;
struct CManager {
    void* pad0;
    CBase** screens;
    char pad8[0x28 - 8];
    int current;
};
CBase* __fastcall CManager_GetCurrentScreen(CManager* self)
{
    CBase** arr = self->screens;
    if (arr[self->current] == 0)
        self->current = 0;
    return arr[self->current];
}