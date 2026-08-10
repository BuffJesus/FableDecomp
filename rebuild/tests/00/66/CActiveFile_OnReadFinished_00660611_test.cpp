#include <stdlib.h>
#include <stdio.h>

struct CActiveFile {
    void* buf;
};

static int g_helper_called = 0;
extern "C" void __fastcall Helper_660611(CActiveFile* self) { g_helper_called++; }

void __fastcall OnReadFinished_660611(CActiveFile* self);

int main() {
    CActiveFile a;
    a.buf = malloc(64);
    g_helper_called = 0;
    OnReadFinished_660611(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }

    CActiveFile b;
    b.buf = 0;
    g_helper_called = 0;
    OnReadFinished_660611(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }

    printf("OK_00660611\n");
    return 0;
}