#include "fable_system_font.h"
#include "fable_system_font_data.h"
CSystemFontCharacter* CSystemFont::GetCharacter(unsigned char character){return &FableSystemFontCharacters_013d28f0[character];}
bool CSystemFontCharacter::Initialise(char* elements){Elements=elements;return true;}
bool CSystemFontCharacter::IsElementSolid(unsigned long x,unsigned long y){char* row=Elements+y*6;if(row[x]==0)return false;return true;}
