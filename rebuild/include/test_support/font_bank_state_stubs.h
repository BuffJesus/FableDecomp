#pragma once
#include "fable_font_state_blocks.h"
// Virtual dependencies for fixtures that test font-bank ownership only.
CFontBankStateBlock::~CFontBankStateBlock() {}
void CFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CFontBankStateBlock::CreateCRC() { return 0; }
void CFontBankStateBlock::Set(const CBankStateBlock*) {}
CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CStreamingFontBankStateBlock::CreateCRC() { return 0; }
void CStreamingFontBankStateBlock::Set(const CBankStateBlock*) {}
