#include "fable_game_text_bank.h"
void FABLE_FASTCALL CBankFileAsyncData::DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data) {
    if(data.Data) data.Data->BankFile.DiscardAsyncData(data);
}
