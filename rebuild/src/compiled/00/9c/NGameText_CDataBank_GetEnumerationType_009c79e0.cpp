#include "fable_game_text_bank.h"
CCharString NGameText::CDataBank::GetEnumerationType(unsigned long type) const {
 if(type==0)return CCharString("EGameText");
 if(type==1)return CCharString("ETextGroup");
 return CCharString("");
}
