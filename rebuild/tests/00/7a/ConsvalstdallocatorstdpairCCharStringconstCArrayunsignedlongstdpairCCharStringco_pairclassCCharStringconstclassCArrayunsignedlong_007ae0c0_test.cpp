// Behaviour model for _Cons_val (0x007ae0c0): copy-construct pair (CCharString, CArray).
#include <cstdio>
#include <cstring>

struct CCharString { char* p; };
struct CArrayUL   { void* a; void* b; void* c; };
struct Pair { CCharString first; CArrayUL second; };

static int g_str_ctor = 0;
static int g_arr_ctor = 0;
static CCharString* last_str_self; static CCharString* last_str_src;
static CArrayUL* last_arr_self;    static CArrayUL* last_arr_src;

static void str_copyctor(CCharString* self, CCharString* src){ g_str_ctor++; last_str_self=self; last_str_src=src; self->p = src->p; }
static void arr_copyctor(CArrayUL* self, CArrayUL* src){ g_arr_ctor++; last_arr_self=self; last_arr_src=src; self->a=src->a; self->b=src->b; self->c=src->c; }

static void Cons_val(Pair* self, Pair* val)
{
    if (self)
    {
        str_copyctor(&self->first, &val->first);
        arr_copyctor(&self->second, &val->second);
    }
}

int main()
{
    Pair dst; std::memset(&dst,0,sizeof(dst));
    Pair src;
    char buf[]="hello";
    src.first.p = buf;
    int xa,xb,xc;
    src.second.a=&xa; src.second.b=&xb; src.second.c=&xc;

    Cons_val(&dst, &src);

    bool ok = true;
    ok &= (g_str_ctor==1 && g_arr_ctor==1);
    ok &= (dst.first.p==buf);
    ok &= (dst.second.a==&xa && dst.second.b==&xb && dst.second.c==&xc);
    // order: string first at +0, array second at +4
    ok &= (last_str_self==&dst.first && last_str_src==&src.first);
    ok &= (last_arr_self==&dst.second && last_arr_src==&src.second);

    // null self -> no construction
    g_str_ctor=0; g_arr_ctor=0;
    Cons_val((Pair*)0, &src);
    ok &= (g_str_ctor==0 && g_arr_ctor==0);

    if (ok) std::printf("CONS_VAL_OK\n");
    else std::printf("FAIL\n");
    return 0;
}