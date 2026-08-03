#include <cstdio>

struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };

// Stand-in for the masked empty-string sentinel global (address 0x0129AAF4).
char g_EmptyCharString_0129AAF4[1] = { 0 };

const char* __fastcall CCharString_operator_char_const_ptr_b(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129AAF4;
    return r->data;
}

int main()
{
    int ok = 1;

    CCharString empty; empty.rep = 0;
    ok &= (CCharString_operator_char_const_ptr_b(&empty) == g_EmptyCharString_0129AAF4);

    StringRep rep; rep.data = (char*)"hello"; rep.len = 5;
    CCharString s; s.rep = &rep;
    ok &= (CCharString_operator_char_const_ptr_b(&s) == rep.data);

    rep.data = (char*)"world!";
    ok &= (CCharString_operator_char_const_ptr_b(&s) == rep.data);

    if (ok)
        std::printf("CCHARSTRING_OPERATOR_CHARPTR_0099E4C0_TEST PASS\n");
    else
        std::printf("CCHARSTRING_OPERATOR_CHARPTR_0099E4C0_TEST FAIL\n");
    return 0;
}