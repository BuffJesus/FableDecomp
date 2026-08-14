#include <stdio.h>
#include <string.h>

struct CCharString { char* p; };
struct LongPair { long first; long second; };
struct MyPair { CCharString first; LongPair second; };

static char storage[64];
static void ctor(CCharString* self, CCharString* src) {
    // model copy-ctor: dup the string pointer/content
    self->p = storage;
    if (src->p) strcpy(storage, src->p);
}

static void Cons_val(MyPair* dst, MyPair* src) {
    if (dst) {
        ctor(&dst->first, &src->first);
        dst->second.first  = src->second.first;
        dst->second.second = src->second.second;
    }
}

int main() {
    char srcstr[] = "hello";
    MyPair src; src.first.p = srcstr; src.second.first = 111; src.second.second = 222;
    MyPair dst; dst.first.p = 0; dst.second.first = 0; dst.second.second = 0;
    Cons_val(&dst, &src);
    int ok = (dst.second.first == 111) && (dst.second.second == 222)
             && dst.first.p && strcmp(dst.first.p, "hello") == 0;
    // null dst must be a no-op (no crash)
    Cons_val(0, &src);
    if (ok) printf("CONSVAL_OK\n");
    else    printf("CONSVAL_FAIL\n");
    return 0;
}