#include <stdio.h>
struct Base{};struct Def{unsigned char base[0x3c];long Type;int BaseCopied;
 void CopyBase(const Base*){BaseCopied=1;}void Copy(const Base*);};
void Def::Copy(const Base* source){CopyBase(source);Type=((const Def*)source)->Type;}
int main(){Def source,target;source.Type=7;target.Type=0;target.BaseCopied=0;
 target.Copy((const Base*)&source);if(target.Type!=7||target.BaseCopied!=1)return 1;
 printf("CREATURE_ABILITY_DEF_COPY PASS\n");return 0;}