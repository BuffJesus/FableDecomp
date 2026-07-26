#include <cstdio>

struct CThingComponentSet {
    void* m_ptr;
    char  m_flag;
};

static int g_freed = 0;
static void* g_last = 0;

extern "C" void __cdecl fable_op_delete(void* p) { g_freed++; g_last = p; }

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    fable_op_delete(self->m_ptr);
    self->m_ptr = 0;
    self->m_flag = 0;
}

int main()
{
    CThingComponentSet s;
    int dummy = 42;
    s.m_ptr = &dummy;
    s.m_flag = 7;
    CThingComponentSet_dtor(&s);
    if (g_freed == 1 && g_last == &dummy && s.m_ptr == 0 && s.m_flag == 0) {
        std::printf("CThingComponentSet_0044f6a0_TEST PASS\n");
        return 0;
    }
    std::printf("CThingComponentSet_0044f6a0_TEST FAIL\n");
    return 1;
}