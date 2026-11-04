#include "fable_string.h"
#include <stdio.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[]="";
int main(){const char* texts[]={0,"","a","aba","[name]","\x80\xff\x80","abc\0hidden","\x01\x7f\xfe\xff","01234567890123456789"};
 union{double align;char data[4];} object;CCharString* s=(CCharString*)&object;
 union{double align;char data[32];} storage;int cases=0;
 for(unsigned i=0;i<sizeof(texts)/sizeof(texts[0]);++i)for(unsigned c=0;c<256;++c){memset(&storage,0xa5,sizeof(storage));*(const char**)&storage=texts[i];*(void**)s=texts[i]?&storage:0;const char* text=texts[i]?texts[i]:"";long expected=-1;
  for(unsigned j=0;;++j){if((unsigned char)text[j]==c){expected=j;break;}if(!text[j])break;}
  char before[32];memcpy(before,&storage,32);if(s->FindCharPos((char)c)!=expected||memcmp(before,&storage,32))return 1;++cases;
 }
 printf("CHARACTER_SEARCH_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
