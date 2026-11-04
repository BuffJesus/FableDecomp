// Behaviour of CDefString::SetString(const CCharString&) 0x009D5060 (and the inline converting ctor): asks
// GDefStringTable (and no other table) to add the given string and stores the
// returned index as TablePos.
#include "fable_def_string.h"
#include <stdio.h>

CDefStringTable GDefStringTable;
static const CDefStringTable* g_table;
static const CCharString* g_text;
static long g_next = 41;
long CDefStringTable::AddString(const CCharString& text)
{
    g_table = this;
    g_text = &text;
    return ++g_next;
}

int main()
{
    static char textStore[4];
    const CCharString& text = *reinterpret_cast<const CCharString*>(textStore);
    CDefString a(text);
    if (a.TablePos != 42 || g_table != &GDefStringTable || g_text != &text) { printf("got %ld\n", a.TablePos); return 1; }
    CDefString b(text);
    if (b.TablePos != 43) { puts("second"); return 1; }
    printf("009d5060_TEST PASS\n");
    return 0;
}