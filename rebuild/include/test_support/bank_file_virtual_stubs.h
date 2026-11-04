#pragma once

// Unused CBankFile virtual slots for isolated fixture vtables.
#include "fable_game_text_bank.h"

void CBankFile::OpenReadWrite(
    const CCharString&, unsigned long, unsigned long) {}
bool CBankFile::OpenFileReadOnly(const CWideString&, unsigned long)
{
    return false;
}
void CBankFile::OpenFileReadWrite(
    const CWideString&, unsigned long, unsigned long) {}
void CBankFile::Close() {}
