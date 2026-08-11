#include <stdio.h>

extern "C" void __cdecl freX(void*);

struct CActiveFile {
    void* m_buf;
    void Notify();
    void OnReadFinished();
};

static int g_notify = 0;
static void* g_freed = (void*)-1;

void CActiveFile::Notify() { g_notify++; }
extern "C" void __cdecl freX(void* p) { g_freed = p; }

int main() {
    char buf[8];
    void* bufp = buf;   // array name decays; no address-of operator

    // case 1: non-null buffer -> Notify called, buffer freed
    CActiveFile a;
    a.m_buf = bufp;
    g_notify = 0; g_freed = (void*)-1;
    a.OnReadFinished();
    bool ok1 = (g_notify == 1) && (g_freed == bufp);

    // case 2: null buffer -> Notify called, nothing freed
    CActiveFile b;
    b.m_buf = 0;
    g_notify = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    bool ok2 = (g_notify == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL n1=%d n2=%d\n", g_notify, g_notify);
    return 1;
}