#include <cstdio>
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
int main()
{
    CBase* slots[4];
    slots[0] = (CBase*)0x1111;
    slots[1] = (CBase*)0x2222;
    slots[2] = (CBase*)0;
    slots[3] = (CBase*)0x4444;
    CManager m;
    m.screens = slots;
    /* current points at a non-empty slot: returns that slot, index unchanged */
    m.current = 1;
    if (CManager_GetCurrentScreen(&m) != (CBase*)0x2222) { std::printf("FAIL a\n"); return 1; }
    if (m.current != 1) { std::printf("FAIL b\n"); return 1; }
    /* current points at empty slot: resets to 0 and returns slot 0 */
    m.current = 2;
    if (CManager_GetCurrentScreen(&m) != (CBase*)0x1111) { std::printf("FAIL c\n"); return 1; }
    if (m.current != 0) { std::printf("FAIL d\n"); return 1; }
    std::printf("CManager_0066e7bd_TEST PASS\n");
    return 0;
}