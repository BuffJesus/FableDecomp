#include "fable_system_font.h"
#include "fable_system_font_data.h"
#include <stdio.h>
#include <string.h>
CSystemFontCharacter FableSystemFontCharacters_013d28f0[256];
int main(){unsigned long font[2]={0xa5a5a5a5,0x39393939};char pixels[256];unsigned scenarios=0;
 for(unsigned i=0;i<256;++i){CSystemFontCharacter* c=reinterpret_cast<CSystemFont*>(font)->GetCharacter(static_cast<unsigned char>(i));if(c!=&FableSystemFontCharacters_013d28f0[i])return 1;++scenarios;}
 CSystemFontCharacter character;
 for(unsigned j=0;j<257;++j){char* p=j==256?0:pixels+j;if(!character.Initialise(p) || character.Elements!=p)return 2;++scenarios;}
 character.Elements=pixels;
 for(unsigned y=0;y<6;++y)for(unsigned x=0;x<6;++x)for(unsigned value=0;value<256;++value){memset(pixels,0,sizeof(pixels));pixels[y*6+x]=static_cast<char>(value);if(character.IsElementSolid(x,y)!=(value!=0))return 3;++scenarios;}
 if(font[0]!=0xa5a5a5a5 || font[1]!=0x39393939)return 4;
 printf("SYSTEM_FONT_LEAVES_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
