#include <cstdio>
#include <cstdlib>

struct SubObj {
    int magic;
    int arg;
    int pad[4];
    void ctor(long a) { magic = 0x1234; arg = (int)a; }
};

static void* Alloc_model(long a) {
    char* base = (char*)malloc(0x20);
    SubObj* p = (SubObj*)(base + 8);
    if (p) p->ctor(a);
    return base;
}

int main() {
    void* r = Alloc_model(42);
    if (r == 0) { printf("FAIL null\n"); return 1; }
    SubObj* p = (SubObj*)((char*)r + 8);
    if (p->magic == 0x1234 && p->arg == 42) {
        printf("ALLOC_OK\n");
    } else {
        printf("FAIL vals %x %d\n", p->magic, p->arg);
    }
    free(r);
    return 0;
}