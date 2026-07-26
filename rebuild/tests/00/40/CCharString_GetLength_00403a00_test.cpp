#include <cstdio>
struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };
long __fastcall CCharString_GetLength(const CCharString* self) {
    StringRep* r = self->rep;
    if (r == 0) return 0;
    return r->len;
}
int main() {
    CCharString empty; empty.rep = 0;
    if (CCharString_GetLength(&empty) != 0) { std::printf("FAIL empty\n"); return 1; }
    StringRep rep; rep.data = (char*)"hello"; rep.len = 5;
    CCharString s; s.rep = &rep;
    if (CCharString_GetLength(&s) != 5) { std::printf("FAIL len\n"); return 1; }
    rep.len = 12345;
    if (CCharString_GetLength(&s) != 12345) { std::printf("FAIL len2\n"); return 1; }
    std::printf("CCharString_00403a00_TEST PASS\n");
    return 0;
}