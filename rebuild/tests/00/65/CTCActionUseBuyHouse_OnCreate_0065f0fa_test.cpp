#include <cstdio>

struct CThing;

struct CIntelligentPointer {
    void* vptr;
    CThing* p;
    void copyctor_4a9730(CIntelligentPointer* other);
    void dtor_a01c10();
};

static int g_ctor_calls = 0;
static int g_dtor_calls = 0;
static CIntelligentPointer* g_last_self = 0;

void CIntelligentPointer::copyctor_4a9730(CIntelligentPointer* other) {
    g_ctor_calls++;
    this->vptr = other->vptr;
    this->p = other->p;
    g_last_self = this;
}
void CIntelligentPointer::dtor_a01c10() {
    g_dtor_calls++;
}

extern "C" CIntelligentPointer* __fastcall CTCActionUseBuyHouse_OnCreate(
        CIntelligentPointer* self, int edx, CIntelligentPointer arg);

int main() {
    CIntelligentPointer self;
    self.vptr = 0; self.p = 0;

    CIntelligentPointer arg;
    arg.vptr = (void*)0xDEADBEEF;
    arg.p = (CThing*)0x12345678;

    CIntelligentPointer* r = CTCActionUseBuyHouse_OnCreate(&self, 0, arg);

    bool ok = (r == &self)
           && (g_ctor_calls == 1)
           && (g_dtor_calls == 1)
           && (self.vptr == (void*)0xDEADBEEF)
           && (self.p == (CThing*)0x12345678)
           && (g_last_self == &self);

    if (ok) {
        printf("ONCREATE_BUYHOUSE_OK\n");
        return 0;
    }
    printf("ONCREATE_BUYHOUSE_FAIL ctor=%d dtor=%d r=%p self=%p\n",
           g_ctor_calls, g_dtor_calls, (void*)r, (void*)&self);
    return 1;
}