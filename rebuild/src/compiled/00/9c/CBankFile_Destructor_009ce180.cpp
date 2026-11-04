#include "fable_game_text_bank.h"
void __fastcall CloseBankFileImpl(CBankFile*);
CBankFile::~CBankFile() { CloseBankFileImpl(this); }
