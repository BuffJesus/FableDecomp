#include "fable_game_text_bank.h"

NGameText::CDataBank::~CDataBank()
{
    CloseTextAndGroups();
}

void NGameText::CDataBank::Close()
{
    CloseTextAndGroups();
    CBankFile::Close();
}
