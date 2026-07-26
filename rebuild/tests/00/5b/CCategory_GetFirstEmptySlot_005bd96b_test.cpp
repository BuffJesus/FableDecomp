#include <cstdio>
struct CCategory {
    char pad[8];
    char* m_begin;
    char* m_end;
};
static int g_predicate;
static char* g_hit;
static char* find_slot(char* begin, char* end, int* pred)
{
    (void)begin; (void)pred;
    if (g_hit) return g_hit;
    return end;
}
long CCategory_GetFirstEmptySlot(CCategory* self)
{
    char* end = self->m_end;
    char* begin = self->m_begin;
    char* found = find_slot(begin, end, &g_predicate);
    if (found != self->m_end)
        return (long)(found - self->m_begin) / 0x14;
    return -1;
}
int main()
{
    char buf[400];
    CCategory c;
    c.m_begin = buf;
    c.m_end = buf + 0x14 * 5;
    g_hit = 0;
    if (CCategory_GetFirstEmptySlot(&c) != -1) { std::printf("FAIL empty\n"); return 1; }
    g_hit = buf + 0x14 * 3;
    if (CCategory_GetFirstEmptySlot(&c) != 3) { std::printf("FAIL idx\n"); return 1; }
    std::printf("CCategory_005bd96b_TEST PASS\n");
    return 0;
}