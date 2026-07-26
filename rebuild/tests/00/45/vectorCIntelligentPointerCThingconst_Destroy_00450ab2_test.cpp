#include <cstdio>
#include <new>
struct T {
    void* vtbl;
    int marker;
    virtual void vdtor(int flags);
};
void __fastcall Vec_Destroy(T* first, T* last, void* al)
{
    (void)al;
    for (; first != last; first = (T*)((char*)first + 0x48))
        first->vdtor(0);
}
static int g_count = 0;
static int g_lastflag = -1;
void T::vdtor(int flags)
{
    g_count++;
    g_lastflag = flags;
}
int main()
{
    char buf[3 * 0x48];
    for (int i = 0; i < 3; i++) new ((void*)(buf + i * 0x48)) T();
    Vec_Destroy((T*)&buf[0], (T*)&buf[3 * 0x48], 0);
    if (g_count == 3 && g_lastflag == 0)
    {
        std::printf("vecdestroy_00450ab2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL count=%d flag=%d\n", g_count, g_lastflag);
    return 1;
}