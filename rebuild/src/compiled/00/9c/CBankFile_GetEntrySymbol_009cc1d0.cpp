#include "fable_game_text_bank.h"
CCharString CBankFile::GetEntrySymbol(unsigned long index) const {
    if (Symbols.empty()) return CCharString("(Unknown)",-1);
    return Symbols[index];
}
