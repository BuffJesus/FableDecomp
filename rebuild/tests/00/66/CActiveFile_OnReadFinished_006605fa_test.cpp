#include <stdlib.h>
#include <stdio.h>

struct CActiveFile {
    void* buf;
};

static int g_pre = 0;
extern "C" void __fastcall CActiveFile_PreRead(CActiveFile* self) { g_pre++; }

void __fastcall OnReadFinished(CActiveFile* self);

int main() {
    CActiveFile a;
    a.buf = malloc(64);
    OnReadFinished(&a);
    CActiveFile b;
    b.buf = 0;
    OnReadFinished(&b);
    if (g_pre == 2) printf("OK_006605fa\n");
    return 0;
}