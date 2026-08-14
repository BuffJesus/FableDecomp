#include <stdio.h>

// Behaviour model of Resize: forwards to two subobject methods in order,
// MethodA gets &fieldA, MethodB gets n.

static int g_log[4];
static int g_i;

struct FieldA { int a; };

struct Sub {
    int s;
    void MethodA(FieldA* arg) { g_log[g_i++] = 1; g_log[g_i++] = arg->a; }
    void MethodB(long n)      { g_log[g_i++] = 2; g_log[g_i++] = (int)n; }
};

struct Ctx {
    char   pad[0x44];
    FieldA fieldA;
    char   pad2[0x4c - 0x44 - sizeof(FieldA)];
    Sub    sub;
    void Resize(long n) {
        this->sub.MethodA(&this->fieldA);
        this->sub.MethodB(n);
    }
};

int main() {
    Ctx c;
    c.fieldA.a = 77;
    c.Resize(9);
    // expect order: MethodA(77) then MethodB(9)
    if (g_log[0]==1 && g_log[1]==77 && g_log[2]==2 && g_log[3]==9)
        printf("RESIZE_OK\n");
    else
        printf("FAIL\n");
    return 0;
}