#include <cstdio>
struct CCharString { void* p; };
static int g_ctor_calls = 0;
static const char* g_last_str = 0;
static int g_last_n = 0;
static void ctor_model(CCharString* self, const char* s, int n) {
    g_ctor_calls++; g_last_str = s; g_last_n = n; self->p = (void*)0xDEAD;
}
static CCharString* model(CCharString* self) {
    ctor_model(self, (const char*)0x123ba5c, -1);
    return self;
}
int main() {
    CCharString buf; buf.p = 0;
    CCharString* r = model(&buf);
    int ok = 1;
    if (r != &buf) ok = 0;
    if (g_ctor_calls != 1) ok = 0;
    if (g_last_str != (const char*)0x123ba5c) ok = 0;
    if (g_last_n != -1) ok = 0;
    if (buf.p != (void*)0xDEAD) ok = 0;
    if (ok) { printf("OK_004d429b\n"); return 0; }
    printf("FAIL_004d429b\n"); return 1;
}