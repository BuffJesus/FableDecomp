#include <stdio.h>
struct Entry{void* A;void* B;float Gain,Offset;bool Cache;unsigned char pad[3];};struct Pair{long ID;Entry EntryData;};
struct Manager{unsigned char base[0x1c];Pair* Begin;Pair* End;void ResetMusicEntriesOffsets();};
void Manager::ResetMusicEntriesOffsets(){for(Pair* p=Begin;p!=End;++p)p->EntryData.Offset=0.0f;}
int main(){Pair p[2];p[0].EntryData.Offset=1.5f;p[1].EntryData.Offset=-2.0f;Manager m;m.Begin=p;m.End=p+2;
 m.ResetMusicEntriesOffsets();if(p[0].EntryData.Offset!=0||p[1].EntryData.Offset!=0)return 1;
 printf("MUSIC_OFFSETS_RESET PASS\n");return 0;}