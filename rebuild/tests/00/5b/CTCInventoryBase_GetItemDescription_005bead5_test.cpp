#include <cstdio>

struct Sub20;
struct CWideString { void* g; };
struct CTCInventoryBase { char pad[0x20]; Sub20* f20; long f24; };

static int gTail = 0;
static void* gLastStack = 0;
static Sub20* gLastSelf = 0;
static long gLastEdx = 0;

void* Str_5d97b0(CWideString* s){ return s->g; }
void* Sub_5c08c0(Sub20* self, long edxArg, void* stackArg){
    gLastSelf = self; gLastEdx = edxArg; gLastStack = stackArg;
    return (void*)0xABCD;
}
void Tail_5be839(void* self){ gTail = (self == (void*)0xABCD) ? 1 : 0; }

void model(CTCInventoryBase* self, CWideString* out, long a, bool b){
    (void)a;(void)b;
    void* r = Str_5d97b0(out);
    void* v = Sub_5c08c0(self->f20, self->f24, r);
    Tail_5be839(v);
}

int main(){
    CTCInventoryBase inv; inv.f20 = (Sub20*)0x1234; inv.f24 = 0x77;
    CWideString ws; ws.g = (void*)0x9999;
    model(&inv, &ws, 5, true);
    if (gLastSelf==(Sub20*)0x1234 && gLastEdx==0x77 && gLastStack==(void*)0x9999 && gTail==1)
        printf("PARITY_OK\n");
    else
        printf("PARITY_FAIL\n");
    return 0;
}