#include <cstdio>

static bool g_freed = false;
static void* g_freed_ptr = 0;
void __cdecl eng_free(void* p) { g_freed = true; g_freed_ptr = p; }

struct CThingComponentSet {
    void* m_ptr;
    unsigned char m_flag;
};

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    eng_free(self->m_ptr);
    self->m_ptr = 0;
    self->m_flag = 0;
}

int main()
{
    int dummy = 42;
    CThingComponentSet s;
    s.m_ptr = &dummy;
    s.m_flag = 7;
    CThingComponentSet_dtor(&s);
    if (g_freed && g_freed_ptr == &dummy && s.m_ptr == 0 && s.m_flag == 0) {
        std::printf("CThingComponentSet_0044c160_TEST PASS\n");
        return 0;
    }
    std::printf("CThingComponentSet_0044c160_TEST FAIL\n");
    return 1;
}