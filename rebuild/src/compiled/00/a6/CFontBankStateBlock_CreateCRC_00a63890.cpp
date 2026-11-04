#include "fable_crc.h"
#include "fable_font_state_blocks.h"

unsigned long CFontBankStateBlock::CreateCRC()
{
    return reinterpret_cast<const CCRC*>(this)->Calc(State, sizeof(State));
}
