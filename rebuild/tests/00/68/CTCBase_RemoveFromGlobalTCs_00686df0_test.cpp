#include <cstdio>

struct CTCBase { char pad[9]; char byte9; };

struct GContainer {
    CTCBase* captured;
    void Remove(CTCBase** pp) { captured = *pp; }
};
GContainer g_container = {0};

void __fastcall RemoveFromGlobalTCs(CTCBase* self)
{
    self->byte9 = 0;
    CTCBase* local = self;
    g_container.Remove(&local);
}

int main()
{
    CTCBase o;
    o.byte9 = 7;
    RemoveFromGlobalTCs(&o);
    if (o.byte9 == 0 && g_container.captured == &o)
        std::printf("00686df0_TEST PASS\n");
    else
        std::printf("FAIL b9=%d cap=%p\n", o.byte9, (void*)g_container.captured);
    return 0;
}