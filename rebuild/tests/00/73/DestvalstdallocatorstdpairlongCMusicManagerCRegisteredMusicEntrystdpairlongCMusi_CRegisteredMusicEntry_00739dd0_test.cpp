#include <cstdio>

struct Sub { void dtor(); int pad0; };
void __fastcall Sub_dtor(Sub* self);

struct PairSlot { void* head; Sub a; Sub b; };
void __fastcall Dest_val(PairSlot* self);

static int g_calls = 0;
static Sub* g_last = 0;
void __fastcall Sub_dtor(Sub* self) { g_calls++; g_last = self; }

int main() {
    PairSlot s;
    s.head = 0;
    Dest_val(&s);
    bool ok = (g_calls == 2) && (s.head == (void*)0x122ffcc)
              && (g_last == &s.a); // last dtor call was on +4
    printf("calls=%d head=%p %s\n", g_calls, s.head, ok ? "PASS_DESTVAL_OK" : "FAIL");
    return ok ? 0 : 1;
}