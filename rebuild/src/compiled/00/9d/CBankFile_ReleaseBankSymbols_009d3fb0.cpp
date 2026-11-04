#include "fable_game_text_bank.h"

// Retail 0043336A is the non-POD string erase instantiation.
#include "fable_stlport_char_string_vector.h"

void __fastcall ReleaseBankSymbols(CArrayVector<CCharString>* symbols)
{
    if (symbols->capacity() > 0) {
        symbols->clear();
        _STL::vector<CCharString> empty;
        symbols->swap(empty);
    }
}
