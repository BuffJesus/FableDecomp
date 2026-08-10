#include <cstdio>

static int g_subDtor = 0;
static int g_base = 0;

struct Sub {
    void Dtor() { g_subDtor++; }
};
struct Obj {
    char pad[0xa8];
    Sub sub;
    void Base() { g_base++; }
    void DestVal_A8_G() {
        this->sub.Dtor();
        this->Base();
    }
};

int main() {
    Obj o;
    o.DestVal_A8_G();
    if (g_subDtor == 1 && g_base == 1) {
        printf("OK_009400f0\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}