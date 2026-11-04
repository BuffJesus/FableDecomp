#pragma once
#include "fable_game_text_bank.h"
// Donor identifies the sole CBankFileAsync base. Retail world-map member
// starts at34 and the following GameWorld pointer is at198.
class CStaticMapBankFile : public CBankFileAsync {
protected:
    virtual unsigned long GetDataFormatVersion();
};
FABLE_STATIC_ASSERT(sizeof(CStaticMapBankFile)==0x164);
