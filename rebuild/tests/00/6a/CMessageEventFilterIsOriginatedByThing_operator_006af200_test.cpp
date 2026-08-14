#include <cstdio>

struct CMessageEvent {
    char pad[0x20];
    unsigned int thing0;
    unsigned int thing1;
};

struct Filter {
    unsigned int a;
    unsigned int b;
    bool operator()(CMessageEvent* ev) {
        if (ev->thing0 != a) return false;
        if (ev->thing1 != b) return false;
        return true;
    }
};

int main() {
    Filter f; f.a = 0x1111; f.b = 0x2222;
    CMessageEvent e; e.thing0 = 0x1111; e.thing1 = 0x2222;
    bool ok = true;
    if (f(&e) != true) ok = false;
    e.thing1 = 0x9999;
    if (f(&e) != false) ok = false;
    e.thing1 = 0x2222; e.thing0 = 0x3333;
    if (f(&e) != false) ok = false;
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}