#include <stdio.h>
struct Base{};struct Def{unsigned char base[0x3c];long PauseFrames;int BaseCopied;
 void CopyBase(const Base*){BaseCopied=1;}void Copy(const Base*);};
void Def::Copy(const Base* source){CopyBase(source);PauseFrames=((const Def*)source)->PauseFrames;}
int main(){Def source,target;source.PauseFrames=12;target.PauseFrames=0;target.BaseCopied=0;
 target.Copy((const Base*)&source);if(target.PauseFrames!=12||target.BaseCopied!=1)return 1;
 printf("WASP_QUEEN_BATTLE_DEF_COPY PASS\n");return 0;}