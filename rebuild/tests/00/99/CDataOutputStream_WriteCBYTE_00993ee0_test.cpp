#include <cstdio>

struct CDataOutputStream;
struct CDOS_vtbl {
    void (*slot0)(CDataOutputStream*);
    void (*slot1)(CDataOutputStream*);
    void (*slot2)(CDataOutputStream*);
    void (*slot3)(CDataOutputStream*);
    void (*Write)(CDataOutputStream*, void*, int);
};
struct CDataOutputStream { CDOS_vtbl* vtbl; };

static int g_size = 0;
static unsigned char g_first = 0;
static void MyWrite(CDataOutputStream*, void* buf, int size) {
    g_size = size;
    g_first = *(unsigned char*)buf;
}

static CDOS_vtbl vt = {0,0,0,0, MyWrite};

void WriteCBYTE_model(CDataOutputStream* self, char value) {
    self->vtbl->Write(self, &value, 4);
}

int main() {
    CDataOutputStream s; s.vtbl = &vt;
    WriteCBYTE_model(&s, (char)0x7A);
    if (g_size == 4 && g_first == 0x7A) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL size=%d first=%02x\n", g_size, g_first);
    }
    return 0;
}