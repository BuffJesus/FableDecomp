#include <cstdio>

struct SymPair;

void __fastcall memb14_dtor(void* self);
void __fastcall memb0c_dtor(void* self);
void __fastcall pair_dtor(SymPair* self);

void __fastcall _Dest_val(SymPair* p, void* al);

static char* g_base;
static int g_seq;
static int g_off14 = -1;
static int g_off0c = -1;
static int g_offpair = -1;
static int g_order14, g_order0c, g_orderpair;

void __fastcall memb14_dtor(void* self)
{
    g_off14 = (int)((char*)self - g_base);
    g_order14 = g_seq++;
}
void __fastcall memb0c_dtor(void* self)
{
    g_off0c = (int)((char*)self - g_base);
    g_order0c = g_seq++;
}
void __fastcall pair_dtor(SymPair* self)
{
    g_offpair = (int)((char*)self - g_base);
    g_orderpair = g_seq++;
}

int main()
{
    char buf[64];
    g_base = buf;
    g_seq = 0;
    _Dest_val((SymPair*)buf, (void*)0);

    bool ok = (g_off14 == 0x14) && (g_off0c == 0xc) && (g_offpair == 0)
              && (g_order14 == 0) && (g_order0c == 1) && (g_orderpair == 2);

    if (ok)
        printf("DESTVAL_OK off14=%d off0c=%d offpair=%d\n", g_off14, g_off0c, g_offpair);
    else
        printf("DESTVAL_FAIL off14=%d off0c=%d offpair=%d seq14=%d seq0c=%d seqp=%d\n",
               g_off14, g_off0c, g_offpair, g_order14, g_order0c, g_orderpair);
    return ok ? 0 : 1;
}