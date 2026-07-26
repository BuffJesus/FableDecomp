#include <cstdio>

static void* g_freed = 0;
extern "C" void eng_free(void* p) { g_freed = p; }

struct CThingComponentSet {
    void* buf;
    char flag;
};

void CThingComponentSet_dtor(CThingComponentSet* self)
{
    eng_free(self->buf);
    self->buf = 0;
    self->flag = 0;
}

static int g_marker = 4660;

int main()
{
    void* ptr = (void*)(&g_marker);
    CThingComponentSet obj;
    obj.buf = ptr;
    obj.flag = 1;
    CThingComponentSet_dtor(&obj);
    if (g_freed == ptr && obj.buf == 0 && obj.flag == 0) {
        std::printf("CThingComponentSet_00450ba0_TEST PASS\n");
        return 0;
    }
    std::printf("CThingComponentSet_00450ba0_TEST FAIL\n");
    return 1;
}