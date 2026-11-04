#include "fable_game_text_bank.h"

// 0x009CD850 is the large bank-file close implementation. Its PDB identity
// is currently misattributed, so keep this edge provisional until recovered.
void __fastcall CloseBankFileImpl(CBankFile* bank);

void CBankFile::Close()
{
    CloseBankFileImpl(this);
}
