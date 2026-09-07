#include "engine/CTCBase.h"
#include <cstdio>


struct GContainer {
    CTCBase* captured;
    void Remove(CTCBase** pp) { captured = *pp; }
};
GContainer g_container = {0};

void __fastcall RemoveFromGlobalTCs(CTCBase* self)
{
    self->IsInGlobalUpdateTCs = 0;
    CTCBase* local = self;
    g_container.Remove(&local);
}

int main()
{
    CTCBase o;
    o.IsInGlobalUpdateTCs = 7;
    RemoveFromGlobalTCs(&o);
    if (o.IsInGlobalUpdateTCs == 0 && g_container.captured == &o)
        std::printf("00686df0_TEST PASS\n");
    else
        std::printf("FAIL b9=%d cap=%p\n", o.IsInGlobalUpdateTCs, (void*)g_container.captured);
    return 0;
}