#include <stdio.h>
#include "rebuild_abi.h"

struct CActiveFile_obj { int dummy; };

extern "C" void __fastcall sub_first(CActiveFile_obj* self, unsigned int a2, char* out) {
    (void)self; (void)a2; if (out) *out = 1;
}
extern "C" void sub_second(CActiveFile_obj* p) { if (p) p->dummy = 42; }

struct CActiveFile {
    CActiveFile_obj* m_obj;
    unsigned int     m_arg;
};

void __fastcall OnReadFinished_0046f191(CActiveFile* thisptr);

int main() {
    CActiveFile_obj obj; obj.dummy = 0;
    CActiveFile f; f.m_obj = &obj; f.m_arg = 7;
    OnReadFinished_0046f191(&f);
    if (obj.dummy == 42) printf("OK_0046f191_PASS\n");

    CActiveFile g; g.m_obj = 0; g.m_arg = 0;
    OnReadFinished_0046f191(&g);
    printf("DONE_0046f191\n");
    return 0;
}