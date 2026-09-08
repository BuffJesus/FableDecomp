#include <stdio.h>
struct Stream{int Count;unsigned long Values[2];Stream():Count(0){}void WriteULONG(unsigned long v){Values[Count++]=v;}};
int main(){Stream s;s.WriteULONG(1);s.WriteULONG(9);if(s.Count!=2||s.Values[0]!=1||s.Values[1]!=9)return 1;printf("MATERIAL_EXT_CHUNK_WRITE PASS\n");return 0;}