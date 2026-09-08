#include <stdio.h>
struct Base{};struct Def{unsigned char base[0x39];bool Dummy;int BaseCopied;
 void CopyBase(const Base*){BaseCopied=1;}void Copy(const Base*);};
void Def::Copy(const Base* source){CopyBase(source);Dummy=((const Def*)source)->Dummy;}
int main(){Def source,target;source.Dummy=true;target.Dummy=false;target.BaseCopied=0;
 target.Copy((const Base*)&source);if(!target.Dummy||target.BaseCopied!=1)return 1;
 printf("THING_GROUP_DEF_COPY PASS\n");return 0;}