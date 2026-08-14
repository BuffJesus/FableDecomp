#include <stdio.h>

struct CharStringData { char* buffer; };
struct CCharString { CharStringData* data; };

static int g_detach_called = 0;
void __fastcall CCharString_Detach(CCharString* /*self*/) { g_detach_called++; }

void __fastcall SetChar(CCharString* self, int, long index, char ch)
{
    if (self->data != 0) {
        CCharString_Detach(self);
        char* base = *(char**)self->data;
        base[index] = ch;
    }
}

int main()
{
    char buf[16] = "AAAAAAAAAAAAAAA";
    CharStringData d; d.buffer = buf;
    CCharString s; s.data = &d;

    SetChar(&s, 0, 3, 'Z');
    int ok = (buf[3] == 'Z') && (buf[2] == 'A') && (g_detach_called == 1);

    CCharString e; e.data = 0;
    SetChar(&e, 0, 5, 'Q');
    ok = ok && (g_detach_called == 1);

    SetChar(&s, 0, 7, 'Y');
    ok = ok && (buf[7] == 'Y') && (g_detach_called == 2);

    if (ok) printf("SETCHAR_OK\n");
    else    printf("SETCHAR_FAIL b3=%c b7=%c dc=%d\n", buf[3], buf[7], g_detach_called);
    return 0;
}