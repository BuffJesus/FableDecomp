#pragma optimize("s",on)
struct CCategory {
    char pad[8];
    char* m_begin;
    char* m_end;
};
extern int g_predicate;
extern char* __fastcall find_slot(char* begin, char* end, int* pred);
long __fastcall CCategory_GetFirstEmptySlot(CCategory* self)
{
    char* end = self->m_end;
    char* begin = self->m_begin;
    char* found = find_slot(begin, end, &g_predicate);
    if (found != self->m_end)
        return (long)(found - self->m_begin) / 0x14;
    return -1;
}