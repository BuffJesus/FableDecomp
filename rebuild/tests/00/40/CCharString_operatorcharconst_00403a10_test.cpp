#include <cstdio>

struct StringRep { char* data; long len; };
struct CCharString { StringRep* rep; };

// Stand-in for the masked empty-string sentinel global (address 0x0129AAF4).
char g_EmptyCharString_0129AAF4[1] = { 0 };

const char* __fastcall CCharString_operator_char_const(const CCharString* self)
{
    StringRep* r = self->rep;
    if (r == 0)
        return g_EmptyCharString_0129AAF4;
    return r->data;
}

int main()
{
    int ok = 1;

    // Null rep -> returns the sentinel's address.
    CCharString empty; empty.rep = 0;
    ok &= (CCharString_operator_char_const(&empty) == g_EmptyCharString_0129AAF4);

    // Non-null rep -> returns rep->data unchanged.
    StringRep rep; rep.data = (char*)"hello"; rep.len = 5;
    CCharString s; s.rep = &rep;
    ok &= (CCharString_operator_char_const(&s) == rep.data);

    rep.data = (char*)"world!";
    ok &= (CCharString_operator_char_const(&s) == rep.data);

    if (ok)
        std::printf("CCHARSTRING_OPERATOR_CHARPTR_00403A10_TEST PASS\n");
    else
        std::printf("CCHARSTRING_OPERATOR_CHARPTR_00403A10_TEST FAIL\n");
    return 0;
}