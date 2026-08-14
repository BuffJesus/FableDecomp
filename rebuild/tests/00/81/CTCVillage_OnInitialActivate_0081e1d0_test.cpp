#include <cstdio>

struct Target { int marker; };

struct CTCVillage {
    Target* obj;
    int     arg2;
    char    pad8[4];
    unsigned char b;
    unsigned char flag;
};

static Target g_captured_obj;
static int    g_captured_arg2;
static int    g_captured_byte;
static int    g_call_count;

void mySub(Target* obj, int arg2, int byteval)
{
    g_captured_obj = *obj;
    g_captured_arg2 = arg2;
    g_captured_byte = byteval;
    g_call_count++;
}

void OnInitialActivate_model(CTCVillage* self)
{
    if (self->flag) {
        mySub(self->obj, self->arg2, self->b);
        self->flag = 0;
    }
}

int main()
{
    Target t; t.marker = 7;
    CTCVillage v;
    v.obj = &t;
    v.arg2 = 99;
    v.b = 0xAB;
    v.flag = 1;

    // flag set -> should call, byte zero-extended, then flag cleared
    OnInitialActivate_model(&v);
    bool ok = (g_call_count == 1) && (g_captured_arg2 == 99)
              && (g_captured_byte == 0xAB) && (g_captured_obj.marker == 7)
              && (v.flag == 0);

    // second call: flag now 0 -> no call
    OnInitialActivate_model(&v);
    ok = ok && (g_call_count == 1);

    if (ok) printf("PARITY_OK_0081e1d0\n");
    else    printf("FAIL\n");
    return 0;
}