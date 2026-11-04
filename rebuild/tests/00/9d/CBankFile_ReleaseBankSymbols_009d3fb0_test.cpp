#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_game_text_bank.h"
void __fastcall ReleaseBankSymbols(CArrayVector<CCharString>*);
static CArrayVector<CCharString>* target;
static CCharString* expectedFirst;
static unsigned erased, destroyed;
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() { ++destroyed; }
template<> CCharString* _STL::vector<CCharString>::erase(CCharString* first, CCharString* last) {
    if (this != target || first != expectedFirst || last != expectedFirst + 3) abort();
    ++erased;
    for (CCharString* p = first; p != last; ++p) p->~CCharString();
    _M_finish = first;
    return first;
}
// Emit the inline specialization so the candidate's external call links here.
CCharString* (_STL::vector<CCharString>::*g_erase)(CCharString*, CCharString*) =
    &_STL::vector<CCharString>::erase;
int main() {
    CArrayVector<CCharString> symbols;
    target = &symbols;
    ReleaseBankSymbols(&symbols);
    if (erased || destroyed || symbols.begin() || symbols.end() || symbols.capacity()) return 1;
    expectedFirst = static_cast<CCharString*>(malloc(4 * sizeof(CCharString)));
    for (unsigned i = 0; i < 3; ++i) new (expectedFirst + i) CCharString;
    symbols._M_set(expectedFirst, expectedFirst + 3, expectedFirst + 4);
    ReleaseBankSymbols(&symbols);
    if (erased != 1 || destroyed != 3 || symbols.begin() || symbols.end() || symbols.capacity()) return 2;
    ReleaseBankSymbols(&symbols);
    if (erased != 1 || destroyed != 3) return 3;
    puts("BANK_SYMBOLS_RELEASE_009d3fb0_PASS");
    return 0;
}
