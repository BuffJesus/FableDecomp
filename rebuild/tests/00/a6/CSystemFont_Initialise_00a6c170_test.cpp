#include "fable_system_font.h"
#include "fable_system_font_data.h"
#include <stdio.h>
#include <string.h>
CSystemFontCharacter FableSystemFontCharacters_013d28f0[256];
bool FableSystemFontInitialized_013d2cf0;
char FableSystemFontBlank_013d2cf4[36];
char FableSystemFontGlyphs_01397c60[60][36];
static const signed char mapping[256]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,54,50,-1,-1,59,57,36,44,45,58,49,41,48,40,38,35,26,27,28,29,30,31,32,33,34,42,43,52,51,53,55,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,46,39,47,-1,37,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,44,56,45,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Retail-executed table oracle.
static unsigned long storage[2],expectedStorage[2];
static char* expected[256];
static unsigned calls,errors,baseResult,mutation;
bool CInitBaseClass::Initialise(){
 if(this!=reinterpret_cast<CInitBaseClass*>(storage) || calls++!=0)++errors;
 reinterpret_cast<unsigned char*>(storage)[4]=static_cast<unsigned char>(baseResult);
 reinterpret_cast<unsigned char*>(expectedStorage)[4]=static_cast<unsigned char>(baseResult);
 if(mutation&1)FableSystemFontInitialized_013d2cf0=!FableSystemFontInitialized_013d2cf0;
 if(mutation&2){FableSystemFontCharacters_013d28f0[19].Elements=FableSystemFontGlyphs_01397c60[59];expected[19]=FableSystemFontGlyphs_01397c60[59];}
 if(baseResult && !FableSystemFontInitialized_013d2cf0)for(unsigned i=0;i<256;++i)expected[i]=mapping[i]<0?FableSystemFontBlank_013d2cf4:FableSystemFontGlyphs_01397c60[mapping[i]];
 return baseResult!=0;
}
int main(){unsigned scenarios=0;
 for(unsigned pattern=0;pattern<32;++pattern)for(baseResult=0;baseResult<2;++baseResult)for(unsigned initialized=0;initialized<2;++initialized)for(mutation=0;mutation<4;++mutation){
  memset(storage,pattern+0x31,sizeof(storage));memcpy(expectedStorage,storage,sizeof(storage));
  memset(FableSystemFontBlank_013d2cf4,0x57,sizeof(FableSystemFontBlank_013d2cf4));memset(FableSystemFontGlyphs_01397c60,0x79,sizeof(FableSystemFontGlyphs_01397c60));
  for(unsigned i=0;i<256;++i)expected[i]=FableSystemFontCharacters_013d28f0[i].Elements=reinterpret_cast<char*>(0x10000000+pattern*1024+i*4);
  FableSystemFontInitialized_013d2cf0=initialized!=0;calls=errors=0;
  bool result=reinterpret_cast<CSystemFont*>(storage)->Initialise();++scenarios;
  if(result!=(baseResult!=0) || calls!=1 || errors || memcmp(storage,expectedStorage,sizeof(storage)) || FableSystemFontInitialized_013d2cf0!=((baseResult || (initialized^((mutation&1)!=0)))!=0))return 1;
  for(unsigned j=0;j<256;++j)if(FableSystemFontCharacters_013d28f0[j].Elements!=expected[j])return 2;
  for(unsigned k=0;k<36;++k)if(FableSystemFontBlank_013d2cf4[k]!=0x57)return 3;
  for(unsigned l=0;l<60*36;++l)if(reinterpret_cast<char*>(FableSystemFontGlyphs_01397c60)[l]!=0x79)return 4;
 }
 printf("SYSTEM_FONT_INIT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
