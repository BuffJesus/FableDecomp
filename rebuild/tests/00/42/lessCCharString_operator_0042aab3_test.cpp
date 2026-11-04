// Behaviour of less<CCharString>::operator() 0x0042AAB3 (map<CCharString, long>'s
// key compare, emitted out of line by the _M_find unit 0x0042B467):
// CCharString::operator< -- shared data is not less, a null string sorts first,
// otherwise CBasicString<char>::operator< 0x00429950 on the text (signed strcmp
// through Compare 0x00411570).
#include "fable_stlport_tree.h"
#include "fable_string.h"
#include <stdio.h>

// Call the landed body, not a COMDAT of this test: the test never names
// less<CCharString>, and the linker resolves LessCall::Call to the landed
// unit's out-of-line operator() (same thiscall signature).
struct LessCall { bool Call(const CCharString& x, const CCharString& y) const; };
#pragma comment(linker, "/alternatename:?Call@LessCall@@QBE_NABVCCharString@@0@Z=??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z")

// A string is its data pointer; the data's first field (CBasicString's PData)
// is the text.
struct Ord { const char* text; };

static const CCharString& Str(void* store, Ord* d)
{
    *reinterpret_cast<Ord**>(store) = d;
    return *reinterpret_cast<CCharString*>(store);
}

int main()
{
    LessCall lc;
    unsigned char a[sizeof(CCharString)], b[sizeof(CCharString)];
    static Ord apple = { "apple" }, appleAgain = { "apple" }, pear = { "pear" }, upper = { "Zebra" };

    if (lc.Call(Str(a, &apple), Str(b, &apple))) { puts("same data"); return 1; }
    if (lc.Call(Str(a, &apple), Str(b, &appleAgain))) { puts("equal text"); return 1; }
    if (!lc.Call(Str(a, &apple), Str(b, &pear))) { puts("apple < pear"); return 1; }
    if (lc.Call(Str(a, &pear), Str(b, &apple))) { puts("pear < apple"); return 1; }
    if (!lc.Call(Str(a, &upper), Str(b, &apple))) { puts("case order"); return 1; }
    if (!lc.Call(Str(a, 0), Str(b, &apple))) { puts("null first"); return 1; }
    if (lc.Call(Str(a, &apple), Str(b, 0))) { puts("null last"); return 1; }
    if (lc.Call(Str(a, 0), Str(b, 0))) { puts("both null"); return 1; }

    puts("FABLETLC_LESS_CCHARSTRING_0042AAB3_PASS");
    return 0;
}
